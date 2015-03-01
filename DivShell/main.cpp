#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <shlwapi.h>
#include <vector>
#include <direct.h>



#include "..\Common\IMod.h"
#include "..\DivGraph\DivGraph.h"
#include "..\DivUserInterface\DivUserInterface.h"

inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}

#include "SYSWindow.h"


LRESULT __stdcall MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

typedef IMod* GetModObject();

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdLine, INT)
{
	Core::Function_Enter("WinMain()");

		IMod* mod;

	HMODULE module = LoadLibrary("..\\Times Of War\\TimesOfWar.dll");
	
	if(!module) return 1;
	HMODULE dev_mod = LoadLibrary("D3DDev.dll");
	if(!dev_mod) return 1;
	DivGraph_Init(dev_mod);
	char path[256];
	GetModuleFileName(module, path, 255);
	char* iter = path + 255;
	while(*iter != '\\')
		--iter;
	iter[1] = 0;
	GetModObject* Game = (GetModObject*)GetProcAddress(module, "Game");
	_chdir(path);
	if(!Game) return 1;
	mod = Game();

	SYSWindow* MainWnd = new SYSWindow;
	MainWnd->Create(100, 100, 640, 480, "Div");
	ShowWindow(MainWnd->hWnd(), SW_SHOWNORMAL);
	UpdateWindow(MainWnd->hWnd());
	mod->Set_hWnd(MainWnd->hWnd());


	int ret;
		
	//try
	{
		mod->Init(DivGraph_GetDevice(), cmdLine);
		ret = mod->Run(DivGraph_GetDevice(), cmdLine);	
	}
	/*catch(...)
	{
		char buff[4096];
		sprintf(buff, "An exception has occured in module:\n%s\n", engine->ECore()->GetFunctionStack());
		OutputDebugString(buff);
		MessageBox(MainWnd->hWnd(), buff, "Exception", MB_OK);
	}*/
	
	
	// kill the window
	//try
	{
		delete mod;
		MainWnd->Cleanup();
		delete MainWnd;
		Core::Function_Exit();
	}
	//catch(...)
	{
	//	OutputDebugString("Error in Cleanup!\n");
	}
	return ret;
}