
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UI_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef _UI_H
#define _UI_H

#ifdef UI_EXPORTS
#define UI_API __declspec(dllexport)
#else
#define UI_API __declspec(dllimport)
#endif

#include <Result.h>
#include "messages.h"

typedef BOOL WndEnumFunc(HANDLE h);
struct DFONTDESC
{
	INT bold;
	BOOL italic;
	BOOL underline;
	DWORD color;
};

struct WNDINFO
{
	POINT	wndPos;
	SIZE	wndSize;
};

struct SCROLLBARDATA
{
	DWORD Max;
	DWORD Val;
};

enum Style
{
	_3D = 0,
	Flat,

	None,
};

struct DELTAPOINT
{
	float x;
	float y;
};



typedef VOID UIMouseProc(INT button, INT x, INT y, BOOL down);
typedef VOID UIKeyProc(char key, BOOL down);

RESULT     UI_API InitUI();
RESULT     UI_API LoadUISettings();
RESULT     UI_API CreateDesktop();
RESULT     UI_API CreateDWindow(char* WindowTitle, MessageProc* MessageProcedure, HANDLE Parent,
								INT x, INT y, INT cx, INT cy, DWORD BackColor, DWORD TitleColor,
								HANDLE TitleTex, HANDLE BodyTex, HANDLE closeText, HANDLE closeOverText, 
								BOOL Title, Style sty, BOOL hasCloseBttn, HANDLE* hWnd);
RESULT     UI_API CreateLabel(int x, int y, int cx, int cy, HANDLE parent, char* text, DWORD color, char* fontname, int size, 
							int bold, BOOL italic, BOOL underline, char* name, HANDLE* hLabel);
RESULT     UI_API UIClick(INT x, INT y);
RESULT     UI_API UIVisible(HANDLE wnd, BOOL v);
BOOL       UI_API UIIsVisible(HANDLE wnd);
BOOL       UI_API UIHasParent(HANDLE wnd);
RESULT     UI_API UIAttachToMouse(HANDLE wnd, int offsetX, int offsetY, BOOL b);
RESULT     UI_API MoveDWindow(HANDLE wnd, int x, int y, int cx, int cy);
RESULT     UI_API GetWindowInfo(HANDLE wnd, WNDINFO info);

RESULT     UI_API LButtonDown(int x, int y);
RESULT     UI_API LButtonUp(int x, int y);
RESULT     UI_API RButtonDown(int x, int y);
RESULT     UI_API RButtonUp(int x, int y);

VOID       UI_API SetKey(char key, BOOL s);
BOOL       UI_API GetKey(char key);

POINT      UI_API GetMousePos();
RESULT     UI_API SetMousePos(POINT p);
RESULT     UI_API SetWindowOffset(POINT p);
RESULT     UI_API UpdateMouse();
RESULT     UI_API GetFontName(char** font);
RESULT     UI_API GetWindowFontDesc(DFONTDESC* fdesc);
RESULT     UI_API GetWindowInfo(HANDLE wnd, WNDINFO* info);
RESULT     UI_API SetTitleTexture(HANDLE wnd, HANDLE text);
RESULT     UI_API SetBodyTexture(HANDLE wnd, HANDLE text);
RESULT     UI_API GetBodyTexture(HANDLE wnd, HANDLE* text);
RESULT     UI_API GetTitleTexture(HANDLE wnd, HANDLE* text);
RESULT     UI_API UIBeginDraw();
RESULT     UI_API UIEndDraw();
RESULT     UI_API SetLabelFormat(HANDLE lbl, DWORD format);
RESULT     UI_API SetLabelText(HANDLE lbl, char* text);
RESULT     UI_API CreateButton(char* text, HANDLE parent, MessageProc* msgproc, int x, int y, int cx,
							   int cy, HANDLE bg, DWORD color, DWORD textColor, char* fontface, int size, 
							   int bold, bool italic, bool underline, Style sty, HANDLE* button);
RESULT     UI_API CreateScrollBar(int x, int y, int cx, int cy, HANDLE Parent, HANDLE LinkObj, HANDLE BGText, 
								  HANDLE ButtonDownTexture, HANDLE ButtonUpTexture, HANDLE ThumbTexture,
								  DWORD BackColor, long Max, HANDLE* Scrollbar);
RESULT     UI_API SetTitleBarHeight(HANDLE hWnd, INT newTBH);
RESULT     UI_API LockedXPosition(HANDLE wnd, BOOL lock);
RESULT     UI_API GetLinkObject(HANDLE wnd, HANDLE* lo);
RESULT     UI_API GetScrollBarData(HANDLE wnd, SCROLLBARDATA* sbData);
RESULT     UI_API CreateDisplayBox(int x, int y, int cx, int cy, HANDLE Parent, char* font, int size, int bold, bool italic,
								   bool underline, char* text, DWORD numLines, DWORD BGColor, DWORD bgScrollBarC, HANDLE sbBG, HANDLE sbUp, HANDLE sbDn,
								   HANDLE sbThumb, bool File, HANDLE* DispBox);
RESULT     UI_API SetDisplayBoxText(HANDLE DispBox, char* nweText);
RESULT     UI_API EnumWindows(WndEnumFunc* func);
RESULT     UI_API SetSBMax(HANDLE Obj, long Max);

DELTAPOINT UI_API GetMouseDelta();
RESULT	   UI_API CreateCursor(HANDLE texture);
RESULT     UI_API DrawCursor();
RESULT     UI_API SetKeyProc(UIKeyProc* newProc);
RESULT     UI_API SetMouseProc(UIMouseProc* newProc);


#include "uiwindow.h"
#include "UIDesktop.h"

UI_API UIDesktop*  GetDesktop();


#endif