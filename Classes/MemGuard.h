#ifndef _MEMGUARD_H
#define _MEMGUARD_H

class MemGuard
{
public:
	MemGuard(void* pp) { p = pp; }
	~MemGuard() {if(p) { delete p; p = NULL; } }
	operator void* () { return p; }
private:
	void* p;

};

#endif