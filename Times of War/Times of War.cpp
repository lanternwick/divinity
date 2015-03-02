// Times of War.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#pragma warning (disable:4231)
#include "Times of War.h"
#include "shlwapi.h"
#include <crtdbg.h>
#include "..\DivCore\Core.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    char* file = NULL;
	int flag;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			// set executable path
			file = new char[128];
			// find executable path
			GetModuleFileName((HINSTANCE)hModule, file, 128);
			PathRemoveFileSpec(file);
			Core::Instance()->SetExecutablePath(file);
			delete file;
			flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
			flag |= _CRTDBG_LEAK_CHECK_DF;
			_CrtSetDbgFlag(flag);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			/*OutputDebugString("Dumping memory leaks from Times of War.dll\n");
			if(!_CrtDumpMemoryLeaks())
			{
				OutputDebugString("No Leaks Found in Times of War.dll\n");
			}*/
			break;
    }
	return TRUE;
	
}

TimesOfWar_Mod* mod = NULL;
dn_networking* net = NULL;

_API IMod* Game()
{
	Core::Function_Enter("TimesOfWar=>Game()");
	if(!mod)
	{
		
		mod = new TimesOfWar_Mod;
	}
	Core::Function_Exit();
	return mod;
}

_API void DestroyGame()
{
	if(mod)
	{
		delete mod;
	}
	mod = NULL;
}

_API dn_networking* Network()
{
	
	if(!net)
	{
		net = new tow_networking;
	}
	return net;
}

_API void DestroyNet()
{
	if(net)
	{
		delete net;
	}
	net = NULL;
}