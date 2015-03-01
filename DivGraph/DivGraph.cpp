// DivGraph.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "IIndexBuffer.h"
#include "IVertexBuffer.h"
#include "..\common\CSmartPtr.h"



#include "DivGraph.h"



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
