#include "render_thread.h"

render_thread::render_thread()
{
}

render_thread::~render_thread()
{
}

void render_thread::run_thread()
{
	while(!m_signal)
	{
		m_mutex.lock();
		// thread code
		m_mutex.unlock();
	}
}
