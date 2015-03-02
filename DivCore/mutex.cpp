#include "mutex.h"

mutex::mutex()
{
	m_m = CreateMutex(NULL, FALSE, NULL);
	m_valid = true;
}

mutex::mutex(const mutex& m)
{
	m_m = m.m_m;
	m_valid = m.m_valid;
}

mutex::~mutex()
{
	if(m_valid)
	{
		CloseHandle(m_m);
	}
	m_valid = false;
}

mutex& mutex::operator = (const mutex& m)
{
	m_m = m.m_m;
	m_valid = m.m_valid;
	return *this;
}

bool mutex::lock(bool block)
{
	if(!m_valid)
	{
		return false;
	}
	if(block)
	{
		WaitForSingleObject(m_m, INFINITE);
		return true;
	}
	else
	{
		return WaitForSingleObject(m_m, 0) == WAIT_OBJECT_0 ? true : false;
	}
}

void mutex::unlock()
{
	if(!m_valid)
	{
		return;
	}
	ReleaseMutex(m_m);
}