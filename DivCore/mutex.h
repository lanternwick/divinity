#ifndef __MUTEX_H
#define __MUTEX_H

#include <windows.h>
#include "..\Common\define_exports.h"

class _API mutex
{
public:
	mutex();
	mutex(const mutex& m);
	~mutex();

	mutex& operator = (const mutex& m);
	void lock();
	void unlock();
	
private:
	CRITICAL_SECTION m_cs;
};

#endif