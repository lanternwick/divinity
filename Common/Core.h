#ifndef _CORE_H
#define _CORE_H

#include <RESULT.h>

class __declspec(dllexport) Core
{
public:
	
	~Core();

	static Core* Instance();

	RESULT InitCore();
	RESULT Destroy();
	RESULT InitDevice();
	RESULT ParseInfoFile(/* add parameters */);

	RESULT Update();

	
	DWORD GetTickCount();

protected:
	Core();
	static Core* _instance;


};

LPVOID __declspec(dllexport) GimmeMem(size_t size);
VOID __declspec(dllexport) FreeMem(void* p);

#endif