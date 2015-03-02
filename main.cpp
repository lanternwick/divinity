#include <windows.h>
#include <assert.h>
#include <direct.h>
#include <FVF.h>
#include <utils.h>
#include <types.h>
#include <stdio.h>
#include <RPTVECTOR.h>

#include "package\package.h"
#include "system\system.h"
#include "ui\ui.h"
#include <CLogFile.h>

BOOL bRunning = FALSE;

typedef RESULT (ModFunc)();
typedef RESULT (SplashScrFunc)(char* name, SIZE* s);

ModFunc* Render;
ModFunc* Update;
ModFunc* InitMod;
ModFunc* DestroyMod;
SplashScrFunc* Splash;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT p;
	switch(msg)
	{
	case WM_DESTROY:
		bRunning = FALSE;
		return 0;
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	case WM_KEYDOWN:
		
		SetKey(wParam, TRUE);
		if(wParam == VK_ESCAPE)
			bRunning = FALSE;
		return 0;
	case WM_KEYUP:
		SetKey(wParam, FALSE);
		return 0;
	case WM_LBUTTONDOWN:
		SetKey(VK_LBUTTON, TRUE);
		return 0;
	case WM_LBUTTONUP:
		SetKey(VK_LBUTTON, FALSE);
		return 0;
	case WM_RBUTTONDOWN:
		SetKey(VK_RBUTTON, TRUE);
		return 0;
	case WM_RBUTTONUP:
		SetKey(VK_RBUTTON, FALSE);
		return 0;
	case WM_MOUSEMOVE:
		
		p.x = LOWORD(lParam);
		p.y = HIWORD(lParam);
		SetMousePos(p);
		//GetCursorPos(&p);
		return 0;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI SplashProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


#include <SYSWindow.h>
#include <result.h>

char* VerifyAndPatchCLine(char* cLine)
{
	WORD period;
	BOOL found = FALSE;
	char* ptr = cLine;
	char isPack[] = ".package";
	char* c = NULL;

	if(strcmp(cLine, "") == 0)
	{
		c = new char[strlen("tow.package")];
		strcpy(c, "tow.package");
		return c;
	}

	for(INT i = 0; *ptr != NULL; ++ptr, ++i)
	{
		if(*ptr == '.')
		{
			period = i;
			found = TRUE;
			break;
		}
	}

	if(!found) // patch command line
	{
		strcat(cLine, ".package");
		c = cLine;
		return c;
	}

	for(i = period; UINT(i) < strlen(isPack); ++i)
	{
		if(cLine[i] != isPack[period - i])
		{
			strcpy(cLine, "tow.package");
			c = cLine;
			return c;
		}
	}
	return cLine;
}


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdline, INT)
{

	RESULT r = OK;
	SYSWindow* Window = NULL;
	HWND SplashWindow;
	HDC SplashImageDC;
	HBITMAP SplashImage;
	WNDCLASSEX wcx;
	char appDir[256];
	ZeroMemory(&wcx, sizeof(wcx));
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_CLASSDC | CS_SAVEBITS;
	wcx.lpfnWndProc = SplashProc;
	wcx.lpszClassName = "SplashWindow";
	RegisterClassEx(&wcx);
	//create library path, for example if the command line 
	//is UIDemo, lib path will be UIDemo\UIDemo.dll
	char* clne;
	BOOL inPack = FALSE;
	clne = new char[128];
	sprintf(clne, "%s\\%s.dll", cmdline, cmdline); 
	HINSTANCE Mod = LoadLibrary(clne);
	if(!Mod) // Relative Path Library couldn't load.  Check some other options
	{
		Mod = LoadLibrary(cmdline);
		if(!Mod) // Absolute Path Library couldn't load.
		{
			sprintf(clne, "%s\\%s.package", cmdline, cmdline);
			r = OpenPackage(clne);
			if(RFAILED(r)) // Relative package path not found...
			{
				r = OpenPackage(cmdline); // Absolute Path package not found, quit
				if(RFAILED(r))
				{
					return 0;
				}
				else
				{
					inPack = TRUE;
				}
			}
			else
			{
				inPack = TRUE;
			}
		}
	}
	if(inPack)
	{
		ExtractAllFiles("cache");
		ClosePackage();
	}
	CLogFile* l = new CLogFile("c:\\divldr.log"); // add file logging
	AddLogRepresentation(l);	
	GetModuleFileName(NULL, appDir, 256);
	OutputLogString(appDir);
	OutputLogString("\n");
	for(int i = strlen(appDir); i > 0; --i)
	{
		if(appDir[i] == '\\')
		{
			appDir[i+1] = 0;
			break;
		}
	}
	_chdir(appDir);
	
	OutputLogString(appDir);
	OutputLogString("\n");
	//setup mod interface functions
	InitMod = (ModFunc*)GetProcAddress((HMODULE)Mod, "InitMod");
	Render = (ModFunc*)GetProcAddress((HMODULE)Mod, "RenderScene");
	Update = (ModFunc*)GetProcAddress((HMODULE)Mod, "UpdateScene");
	DestroyMod = (ModFunc*)GetProcAddress((HMODULE)Mod, "DestroyMod");
	Splash = (SplashScrFunc*)GetProcAddress((HMODULE)Mod, "Splash");

	char splpath[256];
	char splipath[256];
	if(Splash)
	{
		SIZE s;
		
		Splash(splipath, &s);
		sprintf(splpath, "%s\\%s", cmdline, splipath);
		SplashImageDC = CreateCompatibleDC(GetDC(GetDesktopWindow()));
		SplashImage = (HBITMAP)LoadImage(NULL, splpath, IMAGE_BITMAP, s.cx, s.cy, LR_LOADFROMFILE);
		SelectObject(SplashImageDC, SplashImage);
		SplashWindow = CreateWindowEx(0, "SplashWindow", "", WS_POPUP, 
									  (GetSystemMetrics(SM_CXSCREEN) / 2) - (s.cx / 2), 
									  (GetSystemMetrics(SM_CYSCREEN) / 2) - (s.cy / 2),
									  s.cx, s.cy, NULL, NULL, NULL, NULL);
		ShowWindow(SplashWindow, SW_SHOWNORMAL);
		UpdateWindow(SplashWindow);
		BitBlt(GetDC(SplashWindow), 0, 0, s.cx, s.cy, SplashImageDC, 0, 0, SRCCOPY);	
		UpdateWindow(SplashWindow);
	}
	
	Sleep(500);

	Window = new SYSWindow;
	HWND Main_hWnd = Window->Create(100, 100, 643+(2*GetSystemMetrics(SM_CXEDGE)), 480+GetSystemMetrics(SM_CYCAPTION)+(2*GetSystemMetrics(SM_CYEDGE)), "Divinity");
	POINT p = {100, 100};
	SetWindowOffset(p);
	
	bRunning = TRUE;
	
	LoadSettings(); // need them settimgs!
	LoadUISettings(); // more settings  =)
	InitSystem(); //gotta initialize System, otherwise, access viol on exit (Mem Manager)

			 // ...
	//OutputLogString(appDir);
	char msgBoxText[256];
	char a[64];
	char b[64];
	//MessageBox(0, "Here3", "", MB_OK);
	r = InitDevice(Main_hWnd); // need a rendering device...
	if(RFAILED(r))
	{
		ResultText(a, r);
		ResultDescription(b, r);
		sprintf(msgBoxText, "%s\n---------\nDescription: %s", a, b);
		MessageBox(Main_hWnd, msgBoxText, "Divinity: Error", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		bRunning = FALSE;
	}
	InitUI();  //Initialize the UI system!

	
	//strcpy(clne, cmdline);
	//clne = VerifyAndPatchCLine(clne);
	//temporary.  Used for testing purposes

	
	

	
	
	//r = OpenPackage(clne);
	if(RFAILED(r))
	{
		//bRunning = FALSE;
	}
	
	//MessageBox(0, "Here2", "", MB_OK);
	DWORD nextUpdate = GetTickCount() + 75;
	SetPath(appDir);
	r = InitMod();
	if(RFAILED(r)) // check for errors
	{
		ResultText(a, r);
		ResultDescription(b, r);
		sprintf(msgBoxText, "%s\n---------\nDescription: %s", a, b);
		MessageBox(Main_hWnd, msgBoxText, "Divinity: Error", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		bRunning = FALSE;
	}
	DestroyWindow(SplashWindow);
	DeleteObject(SplashImage);
	ShowWindow(Main_hWnd, SW_SHOWNORMAL);
	UpdateWindow(Main_hWnd);
	
	//MessageBox(0, "Here", "", MB_OK);

	MSG msg;	
	while(bRunning)
	{
		if(PeekMessage(&msg, Main_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		r = Update();
		if(RFAILED(r)) // check for errors
		{
			ResultText(a, r);
			ResultDescription(b, r);
			sprintf(msgBoxText, "%s\n---------\nDescription: %s", a, b);
			MessageBox(Main_hWnd, msgBoxText, "Divinity: Error", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		}
		UpdateMouse();
		GameTick();
		
		r = Render();
		if(RFAILED(r)) // check for errors
		{
			ResultText(a, r);
			ResultDescription(b, r);
			sprintf(msgBoxText, "%s\n---------\nDescription: %s", a, b);
			MessageBox(Main_hWnd, msgBoxText, "Divinity: Error", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		}
		FrameTick();
		
		
	}

	DestroyMod();
	DestroySystem(); //gotta destroy the system too, Duh!
	FreeLibrary((HMODULE)Mod); // Kill the mod DLL
	SaveSystemSettings();
	return 0;
}