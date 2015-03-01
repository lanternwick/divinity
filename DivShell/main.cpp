#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <shlwapi.h>
#include <vector>

#pragma warning ( disable : 4231 )
#include "..\DivEngine\DivEngine.h"
#include "..\Common\matrix.h"
#include "..\Common\IMod.h"

inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}

CEngine* engine = NULL;

#include "SYSWindow.h"


LRESULT __stdcall MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	case WM_DESTROY:
		engine->bRunning(FALSE);
		return 0;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			engine->bRunning(FALSE);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

typedef IMod* GetModObject();

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdLine, INT)
{
	Core::Function_Enter("WinMain()");

	engine = new CEngine;

	IMod* mod;

	HMODULE module = LoadLibrary("..\\Times Of War\\TimesOfWar.dll");
	if(!module) return 1;
	GetModObject* Game = (GetModObject*)GetProcAddress(module, "Game");
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
		ret = mod->Run(cmdLine);	
	}
	/*catch(...)
	{
		char buff[4096];
		sprintf(buff, "An exception has occured in module:\n%s\n", engine->ECore()->GetFunctionStack());
		OutputDebugString(buff);
		MessageBox(MainWnd->hWnd(), buff, "Exception", MB_OK);
	}*/
	
	
	// kill the window
	delete mod;
	MainWnd->Cleanup();
	delete MainWnd;
	engine->ECore()->Function_Exit();
	delete engine;
	return ret;
}