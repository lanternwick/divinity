
#include <windows.h>
#include "DivUserInterface.h"
#include "UIInputHandler.h"

UIInputHandler handler;

BOOL __stdcall DllMain(HANDLE hModule, unsigned long ui_reason_for_call, void* lpReserved)
{
	return true;
}

_API void DivUserInterface_Init(bool own_input_handler)
{
	UserInterface* ui = UserInterface::Instance();
	if(!own_input_handler)
		return;
	handler.SetUI(ui);
	ui->SetInputHandler(&handler);
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