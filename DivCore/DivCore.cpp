// DivCore.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "DivCore.h"
#include <crtdbg.h>


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    int flag;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			flag |= _CRTDBG_LEAK_CHECK_DF;
			_CrtSetDbgFlag(flag);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			
			/*OutputDebugString("Dumping memory leaks from Core.dll\n");
			if(!_CrtDumpMemoryLeaks())
			{
				OutputDebugString("No Leaks Found in Core.dll\n");
			}*/
			break;
    }
    return TRUE;
}

// define new and delete operators
/*
inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}*/