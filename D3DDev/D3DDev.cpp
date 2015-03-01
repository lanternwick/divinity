// D3DDev.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "D3DDev.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}

D3DDevice device;
D3DGraphObjFactory factory;

_API IDevice* GetDevice()
{
	return &device;
}

_API IGraphObjFactory* GetFactory()
{
	return &factory;
}