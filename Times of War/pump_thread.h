#ifndef PUMP_THREAD_H
#define PUMP_THREAD_H

#include "..\DivCore\thread.h"

class pump_thread : public thread
{
public:
	pump_thread(unsigned long hwnd);
	~pump_thread();
private:
	unsigned long m_hwnd;
	void run_thread();
};

#endif
