// Times of War.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#pragma warning (disable:4231)
#include "Times of War.h"
#include "shlwapi.h"

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
	// set executable path
	char* file = new char[128];
	// find executable path
	GetModuleFileName((HINSTANCE)hModule, file, 128);
	PathRemoveFileSpec(file);
	Core::Instance()->SetExecutablePath(file);
    return TRUE;
}

TimesOfWar_Mod* mod = NULL;

TIMESOFWAR_API IMod* Game()
{
	if(!mod)
	{
		
		mod = new TimesOfWar_Mod;
	}
	return mod;
}

