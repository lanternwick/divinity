#include "thread.h"
#include <process.h>

thread::thread()
{
	m_signal = false;
	m_thread_id = NULL;
}

thread::~thread()
{
	m_signal = true;
	if(m_thread_id)
	{
		_cwait(NULL, m_thread_id, 0);
		m_thread_id = NULL;
		m_signal = false;
	}
}

void thread::begin(bool create_suspended)
{
	if(m_thread_id)
	{
		return;
	}
	if(create_suspended)
	{
		suspend();
	}
	m_thread_id = _beginthread(thread_proc, 0, this);
}

void thread::resume()
{
	m_mutex.unlock();
}

void thread::suspend()
{
	m_mutex.lock();
}

void thread::thread_proc(void* param)
{
	thread* t = (thread*)param;
	t->run_thread();
}
