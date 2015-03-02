#include "pump_thread.h"
#include <windows.h>
#include "..\DivCore\Core.h"

pump_thread::pump_thread(unsigned long hwnd)
{
	thread::thread();
	m_hwnd = hwnd;
	m_signal = false;
}

pump_thread::~pump_thread()
{

}

void pump_thread::run_thread()
{
	MSG msg;
	while(!m_signal)
	{
		m_mutex.lock();
		if(PeekMessage(&msg, (HWND)m_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		m_mutex.unlock();
	}
}