#include "BinaryData.h"

#include "..\console\overloads.h"

custom::binary_data::binary_data()
{
	m_size = 0;
	data = 0;
}

custom::binary_data::~binary_data()
{
	m_size = 0;
	if(data) delete data;
	data = 0;
}

unsigned long custom::binary_data::resize(unsigned long newSize)
{
	
	if(newSize==0)
	{
		if(data)
		{
			delete data;
		}
		data = 0;
		m_size = 0;
		return 0;
	}
	if(m_size == 0)
	{
		data = new char[newSize];
		m_size = newSize;
		return m_size;
	}
	char* temp = new char[newSize];
	unsigned long copy = newSize > m_size ? m_size : newSize;
	memcpy(temp, data, copy);
	delete data;
	data = temp;
	m_size = newSize;
	return newSize;
}

void* custom::binary_data::extract(void* out, unsigned long size, unsigned long offset)
{
	if(!out) return out;
	if(size+offset>m_size) return 0;
	memcpy(out, data+offset, size);
	return out;
}

void custom::binary_data::insert(void* in, unsigned long size, unsigned long offset)
{
	if(!in) return;
	if(size+offset > m_size) return;
	memcpy(data+offset, in, size);
}

unsigned long custom::binary_data::size()
{
	return m_size;
}

bool custom::binary_data::operator == (custom::binary_data& rhs)
{
	if(rhs.m_size != m_size) return false;
	return memcmp(data, rhs.data, m_size)==0;
}

bool custom::binary_data::operator != (custom::binary_data& rhs)
{
	return !(rhs == *this);
}

bool custom::binary_data::operator > (custom::binary_data& rhs)
{
	if(rhs.m_size != m_size) return false;
	return memcmp(data, rhs.data, m_size)==1;
}

bool custom::binary_data::operator < (custom::binary_data& rhs)
{
	if(rhs.m_size != m_size) return false;
	return memcmp(data, rhs.data, m_size)==-1;
}

bool custom::binary_data::operator >= (custom::binary_data& rhs)
{
	return !(*this<rhs);
}

bool custom::binary_data::operator <= (custom::binary_data& rhs)
{
	return !(*this>rhs);
}

custom::binary_data& custom::binary_data::operator = (const custom::binary_data& rhs)
{
	if(data) delete data;
	m_size = rhs.m_size;
	
	if(m_size)
	{
		data = new char[rhs.m_size];
		memcpy(data, rhs.data, m_size);
	}
	return *this;
}

custom::binary_data& custom::binary_data::operator +=(custom::binary_data& rhs)
{
	if(!rhs.m_size) return *this;
	
	unsigned long off = m_size;
	resize(m_size+rhs.m_size);
	insert(rhs.data, rhs.m_size, off);
	return *this;
}

custom::binary_data::operator const void* () const
{
	return (void*)data;
}

custom::binary_data::operator void* ()
{
	return (void*)data;
}