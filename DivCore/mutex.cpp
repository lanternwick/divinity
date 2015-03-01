#include "mutex.h"

mutex::mutex()
{
	InitializeCriticalSection(&m_cs);
}

mutex::mutex(const mutex& m)
{
	m_cs = m.m_cs;
}

mutex::~mutex()
{
	DeleteCriticalSection(&m_cs);
}

mutex& mutex::operator = (const mutex& m)
{
	m_cs = m.m_cs;
	return *this;
}

void mutex::lock()
{
	EnterCriticalSection(&m_cs);
}

void mutex::unlock()
{
	LeaveCriticalSection(&m_cs);
}