#include "PostOffice.h"




MailMessage::MailMessage()
{
	m_message_data = NULL;
	m_message_len = 0;
	m_message_id = 0;
	m_t = 0.0f;
	m_server_stamp = 0;
}

MailMessage::MailMessage(const MailMessage& m)
{
	m_message_dest = m.m_message_dest;
	m_message_id = m.m_message_id;
	m_message_len = m.m_message_len;
	if(m_message_len)
	{
		memcpy(m_message_data, m.m_message_data, m_message_len);
	}
	else
	{
		m_message_data = NULL;
	}
	m_t = m.m_t;
	m_server_stamp = m.m_server_stamp;
	
}

MailMessage::MailMessage(moniker& dest, unsigned long id, float t, unsigned long len, void* data, unsigned long server_stamp)
{
	m_message_dest = dest;
	m_message_id = id;
	m_t = t;
	m_message_len = len;
	if(len)
	{
		m_message_data = new char[len];
		memcpy(m_message_data, data, len);
	}
	else
	{
		m_message_data = NULL;
	}
	m_server_stamp = server_stamp;
}

MailMessage::~MailMessage()
{
	m_message_dest = moniker();
	m_message_id = 0;
	m_t = 0.0f;
	m_message_len = 0;
	if(m_message_data)
		delete [] m_message_data;
	m_message_data = NULL;
	m_server_stamp = 0;
}

moniker MailMessage::message_dest() const
{
	return m_message_dest;
}

unsigned long MailMessage::message_id() const
{
	return m_message_id;
}

float MailMessage::t() const
{
	return m_t;
}

unsigned long MailMessage::len() const
{
	return m_message_len;
}

void* MailMessage::message_data()
{
	return m_message_data;
}

unsigned long MailMessage::server_stamp() const
{
	return m_server_stamp;
}

custom::binary_data MailMessage::Serialize() const
{
	custom::binary_data b;
	b.resize(4+4+4+4+m_message_dest().size()+1+m_message_len);
	unsigned long l = 0;
	b.insert(&l, 4);
	int o = 4;
	b.insert((void*)&m_message_id, 4, o);
	o+=4;
	b.insert((void*)&m_message_len, 4, o);
	o+=4;
	b.insert((void*)&m_t, 4, o);
	o+=4;
	l = m_message_dest().size();
	b.insert((void*)&l, 4, o);
	o+=4;
	b.insert((void*)m_message_dest().c_str(), m_message_dest().size()+1, o);
	o+=m_message_dest().size()+1;
	
	b.insert((void*)m_message_data, m_message_len, o);
	return b;
}

void MailMessage::Deserialize(char* raw_message, unsigned long len)
{
	
	unsigned long l;
	memcpy(&m_server_stamp, raw_message, 4);
	memcpy(&m_message_id, raw_message+4, 4);
	memcpy(&m_message_len, raw_message+4, 4);
	memcpy(&m_t, raw_message+8, 4);
	memcpy(&l, raw_message+12, 4);
	char* buffer = new char[l];
	memcpy(buffer, raw_message+16, l);
	
	buffer[l] = 0;
	m_message_dest() = std::string(buffer);
	memcpy(&m_message_data, raw_message+16+l, len-16-l);
}

bool MailMessage::operator > (const MailMessage& m) const
{
	return m_t > m.m_t;
}

dn_network* MailBox::m_network = NULL;

MailBox::MailBox()
{
	m_lock.lock();
	m_lock.unlock();
}

MailBox::MailBox(moniker& owner, bool local)
{
	m_lock.lock();
	m_owner = owner;
	local_box = local;
	m_lock.unlock();
}

MailBox::~MailBox()
{
	m_lock.lock();
	while(!m_messages.empty())
		m_messages.pop();
	m_lock.unlock();
}

void MailBox::DispatchMessage(MailMessage& m)
{
	m_lock.lock();
	if(m.server_stamp())
	{
		m_messages.push(m);
	}
	else
	{
		custom::binary_data bm = m.Serialize();
		custom::binary_data bf;
		std::string s("/mail\n");
		char id[3];
		sprintf(id, "%i\n", m_network->GetId());
		s += id;
		bf.resize(s.size());
		bf += bm;

		if(m_network)
		{
			m_network->SendData((char*)(void*)bf, bf.size());
		}
	}
	m_lock.unlock();
}

MailMessage MailBox::NextMessage()
{
	m_lock.lock();
	MailMessage m;
	m = m_messages.top();
	if(m.t() <= Core::GetTime())
	{
		m_messages.pop();
		m_lock.unlock();
		return m;
	}
	m_lock.unlock();
	return MailMessage();
}

MailMessage& MailBox::PeekMessage()
{
	return m_messages.top();
}

moniker MailBox::Owner() const
{
	return m_owner;
}

bool MailBox::IsEmpty() const
{
	//m_lock.lock();
	MailMessage m;
	m = m_messages.top();
	
	if(m.t() <= Core::GetTime())
	{
		//m_lock.unlock();
		return false;
	}
	//m_lock.unlock();
	return true;
}


void MailBox::SetNetwork(dn_network* net)
{
	m_network = net;
}

PostOffice::PostOffice()
{
	
}

PostOffice::~PostOffice()
{

}

MailBox* PostOffice::CreateMailbox(moniker& owner, bool local)
{
	m_lock.lock();
	MailBox m(owner, local);
	m_boxes.push_back(owner);
	m_lock.unlock();
	return &m_boxes.back();
}

void PostOffice::DestroyMailbox(moniker& owner)
{
	m_lock.unlock();
	int i = 0;
	for(;i < m_boxes.size(); ++i)
	{
		if(owner() == m_boxes[i].Owner()())
		{
			m_boxes[i] = m_boxes.back();
			m_boxes.pop_back();
			m_lock.unlock();
			return;
		}
	}
	m_lock.unlock();
}

void PostOffice::SendMessage(moniker& dest, unsigned long message_id, unsigned long len, void* data, float t, unsigned long server_stamp)
{
	m_lock.lock();
	MailMessage m(dest, message_id, t, len, data, server_stamp);
	SendMessage(m);
	m_lock.unlock();
}

void PostOffice::SendMessage(MailMessage& mm)
{
	m_lock.lock();
	int i = 0;
	for(;i < m_boxes.size(); ++i)
	{
		if(mm.message_dest()() == m_boxes[i].Owner()())
		{
			m_boxes[i].DispatchMessage(mm);			
			m_lock.unlock();
			return;
		}
	}
	m_lock.unlock();
}