#ifndef POSTOFFICE_H
#define POSTOFFICE_H

#pragma warning(disable:4786)

#include "..\DivCore\DivCore.h"
#include <vector>
#include <queue>
#include "..\console\binarydata.h"
#include "tow_networking.h"
#include "..\DivCore\mutex.h"
//#include <utilities>

class MailMessage
{
public:
	MailMessage();
	~MailMessage();
	MailMessage(const MailMessage& m);
	MailMessage(moniker& dest, unsigned long id, float t, unsigned long len, void* data, unsigned long server_stamp=0);
	moniker message_dest() const;
	unsigned long message_id() const;
	void* message_data();
	float t() const;
	unsigned long len() const;
	unsigned long server_stamp() const;
	custom::binary_data Serialize() const;
	void Deserialize(char* raw_mesasge, unsigned long len);
	bool operator > (const MailMessage& m) const;
	
private:
	moniker m_message_dest;
	unsigned long m_message_id;
	void* m_message_data;
	float m_t;
	unsigned long m_message_len;
	unsigned long m_server_stamp;
};

class MailBox
{
public:
	MailBox();
	~MailBox();
	MailBox(moniker& owner, bool local=true);

	void DispatchMessage(MailMessage& m);
	MailMessage NextMessage();
	MailMessage& PeekMessage();
	moniker Owner() const;
	bool IsEmpty() const;
	static void SetNetwork(dn_network* net);
private:
	std::priority_queue<MailMessage, std::vector<MailMessage>, std::greater<MailMessage> > m_messages;
	moniker m_owner;
	bool local_box;
	static dn_network* m_network;
	mutex m_lock;
};

class PostOffice
{
public:
	PostOffice();
	virtual ~PostOffice();

	virtual MailBox* CreateMailbox(moniker& owner, bool local=true);
	virtual void DestroyMailbox(moniker& owner);
	virtual void SendMessage(moniker& dest, unsigned long message_id, unsigned long len, void* data, float t=0.0f, unsigned long server_stamp=0);
	virtual void SendMessage(MailMessage& mm);
private:
	std::vector<MailBox> m_boxes;
	std::vector<std::pair<moniker, MailMessage> > m_messages_to_deliver;
	mutex m_lock;
};

#endif