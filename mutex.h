#ifndef __MUTEX_H
#define __MUTEX_H

#include <windows.h>

class mutex
{
public:
	mutex();
	mutex(const mutex& m);
	~mutex();

	mutex& operator = (const mutex& m);
	bool lock(bool block=true);
	void unlock();
	
private:
	HANDLE m_m;
	bool m_valid;
};

#endif