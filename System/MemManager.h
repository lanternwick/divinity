#ifndef _MEMMANAGER_H
#define _MEMMANAGER_H

#include <malloc.h>

class MemManager
{
public:
	MemManager();
	~MemManager();

	void* GimmeMem(DWORD size);
	void ReleaseMem(void* p);

private:
	DWORD memTotal;
	DWORD memAllocations;
};

MemManager::MemManager()
{
	memTotal = 0;
	memAllocations = 0;
}

MemManager::~MemManager()
{
	char a[128];
	sprintf(a, "Total Mem Allocated: %i\nOutstanding Allocations: %i\n",memTotal, memAllocations); 
	OutputDebugString(a);
}

void* GimmeMem(DWORD size)
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

void ReleaseMem(void* p)
{
	free(p);
	--memAllocations;
	return;
}

#endif
