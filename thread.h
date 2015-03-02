/* Header file for thread object

on object destruction, m_signal is toggled, and run_thread() should exit.

*/

#ifndef __THREAD_H
#define __THREAD_H


#include "mutex.h"

#define THREAD_CREATE_SUSPENDED true

class thread
{
public:
	thread();
	virtual ~thread();
	void begin(bool exit_on_destructor=true, bool create_suspended=false);
	void resume();
	void suspend();
private:
	static void __cdecl thread_proc(void* param);
	virtual void run_thread()=0;
	unsigned long m_thread_id;
protected:
	bool m_exit_on_destructor;
	bool m_signal;
	mutex m_mutex;
};

#endif