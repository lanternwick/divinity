
#include <windows.h>
#include <crtdbg.h>
#include "DivUI.h"
#include "UIInputHandler.h"
#include "..\Common\singleton.h"
UIInputHandler handler;

BOOL __stdcall DllMain(HANDLE hModule, unsigned long ul_reason_for_call, void* lpReserved)
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
			OutputDebugString("Dumping memory leaks from DivUI.dll\n");
			if(!_CrtDumpMemoryLeaks())
			{
				OutputDebugString("No Leaks Found in DivUI.dll\n");
			}
			break;
    }
	return true;
}

_API void DivUI_Init(bool own_input_handler)
{
	UserInterface* ui = new UserInterface;
	singleton<UserInterface*> sui(ui);
	if(!own_input_handler)
		return;
	handler.SetUI(ui);
	ui->Input().SetInputHandler(&handler);
}

_API void DivUI_InitEx(bool own_input_handler, UserInterface* ui)
{
	singleton<UserInterface*> sui(ui);
	if(!own_input_handler)
		return;
	handler.SetUI(ui);
	ui->Input().SetInputHandler(&handler);
}
