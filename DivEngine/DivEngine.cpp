#include "stdafx.h"

#include "DivEngine.h"

// define new and delete operators

inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}



BOOL __stdcall DllMain(HANDLE hModule, 
					  DWORD ul_reason_for_call, 
					  LPVOID lpReserved)
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

