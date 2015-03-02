#ifndef _SYSWINDOW_H
#define _SYSWINDOW_H

#include <windows.h>

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class SYSWindow
{
public:
	SYSWindow();
	~SYSWindow();
	virtual HWND Create(int x, int y, int cx, int cy, LPCTSTR WindowName);
	//LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND hWnd();
	VOID Cleanup();
protected:
	HWND my_hWnd;
	WNDCLASSEX myClass;
};

SYSWindow::SYSWindow()
{
}

SYSWindow::~SYSWindow()
{
	Cleanup();
}

HWND SYSWindow::Create(int x, int y, int cx, int cy, LPCTSTR WindowName)
{
	WNDCLASSEX WndClass = {sizeof(WNDCLASSEX), 
						   CS_CLASSDC | CS_DBLCLKS, 
						   MsgProc, 
						   NULL, 
						   NULL,
						   GetModuleHandle(NULL), 
						   NULL, 
						   NULL, 
						   NULL, 
						   NULL, 
						   "SYSWindow", 
						   NULL
							};
	RegisterClassEx(&WndClass);

	my_hWnd = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_STATICEDGE, 
							 "SYSWindow", 
							 WindowName, 
							 0,
							 x,
							 y,
							 cx,
							 cy,
							 GetDesktopWindow(),
							 NULL,
							 WndClass.hInstance,
							 NULL);
	//UpdateWindow(my_hWnd);
	myClass = WndClass;
	RECT rct;
	rct.top = y;
	rct.left = x;
	rct.bottom = y + cy;
	rct.right = x + cx;
	//ClipCursor(&rct);
	return my_hWnd;
}

HWND SYSWindow::hWnd()
{
	return my_hWnd;
}

VOID SYSWindow::Cleanup()
{
	ClipCursor(NULL);
	UnregisterClass("Window", myClass.hInstance);
}


#endif

