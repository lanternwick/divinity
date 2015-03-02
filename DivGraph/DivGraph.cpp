// DivGraph.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "IIndexBuffer.h"
#include "IVertexBuffer.h"




#include "DivGraph.h"
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
			OutputDebugString("Dumping memory leaks from DivGraph.dll\n");
			if(!_CrtDumpMemoryLeaks())
			{
				OutputDebugString("No Leaks Found in DivGraph.dll\n");
			}
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

typedef IDevice* GetDevice();
typedef IGraphObjFactory* GetFactory();

GetDevice* fnGetDevice;
GetFactory* fnGetFactory;

_API void DivGraph_Init(HMODULE device_dll)
{
	fnGetDevice = (GetDevice*)GetProcAddress(device_dll, "GetDevice");
	fnGetFactory = (GetFactory*)GetProcAddress(device_dll, "GetFactory");
}

_API IDevice* DivGraph_GetDevice()
{
	return fnGetDevice();
}

_API IGraphObjFactory* DivGraph_GetFactory()
{
	return fnGetFactory();
}