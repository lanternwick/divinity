// D3DDev.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "D3DDev.h"
#include <crtdbg.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			if(!_CrtDumpMemoryLeaks())
			{
				OutputDebugString("No Leaks Found in D3DDev.dll\n");
			}
			break;
    }
    return TRUE;
}

/*inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}*/

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