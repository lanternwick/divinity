#ifndef _MEMMANAGER_H
#define _MEMMANAGER_H

#include <malloc.h>
#include <windows.h>
#include <stdio.h>

class MemManager
{
public:
	MemManager();
	~MemManager();

	void* GimmeMem(DWORD size);
	void ReleaseMem(void* p);

	char* owner;

private:
	DWORD memTotal;
	DWORD memAllocations;
};

MemManager::MemManager()
{
	memTotal = 0;
	memAllocations = 0;
	owner = (char*)malloc(128);
	owner[0] = NULL;
}

MemManager::~MemManager()
{
	char a[128];
	sprintf(a, "%s Total Mem Allocated: %i\nOutstanding Allocations: %i\n",owner, memTotal, memAllocations); 
#ifdef _SYSTEM_H
	OutputLogString(a);
#endif
	delete owner;
}

void* MemManager::GimmeMem(DWORD size)
{
	void* p = NULL;
	p = malloc(size);
	
	if(p)
	{
		memTotal+=size;
		++memAllocations;
	}
	return p;
}

void MemManager::ReleaseMem(void* p)
{
	free(p);
	--memAllocations;
	return;
}

#endif
