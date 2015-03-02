// UI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "..\system\system.h"



#include "messages.h"

#include "UI.h"

#include "UIWindow.h"
#include "UIDesktop.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UIScrollBar.h"
#include "UIDisplayBox.h"
#include "UICursor.h"
#include <vector>
#include <fstream.h>
#include <fvf.h>

BOOL keys[256];
HMODULE mod;
POINT mousepos;
POINT offset;
POINT lastMousePos;
DELTAPOINT mouseDelta;
char* fontname = NULL;
DFONTDESC wndFontDesc;
UIDesktop* Desktop = NULL;
char* common = NULL;
UIMouseProc* currMouseProc = NULL;
UIKeyProc* currKeyProc = NULL;
HANDLE cursor;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    mod = (HMODULE)hModule;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			OutputLogString("USER INTERFACE: :====> ENTER: DLLMAIN() : Process Attach\n\n");
			{
				RegisterModule((HMODULE)hModule);
				for(INT i = 0; i < 256; ++i)
					keys[i] = FALSE;
			}
			OutputLogString("USER INTERFACE: :====> EXIT: DLLMAIN() : Process Attach\n\n");
			break;
		case DLL_THREAD_ATTACH: break;
		case DLL_THREAD_DETACH: break;
		case DLL_PROCESS_DETACH: break;
			OutputLogString("USER INTERFACE: :====> ENTER: DLLMAIN() : Process Detach\n\n");
			OutputLogString("USER INTERFACE: :====> EXIT: DLLMAIN() : Process Detach\n\n");
			break;
    }
	
    return TRUE;
}

RESULT UI_API InitUI()
{
	common = new char[256];
	return OK;
}

RESULT UI_API LoadUISettings()
{
	//Open settings
	FILE* f;
	char a[16];
	f = fopen("ui.cfg", "r+");
	//init vars
	fontname = new char[128];
	//read settings
	fgets(fontname, 128, f);
	//close up
	fgets(a, 16, f);
	wndFontDesc.bold = atoi(a);
	fgets(a, 16, f);
	wndFontDesc.italic = atoi(a);
	fgets(a, 16, f);
	wndFontDesc.underline = atoi(a);
	fgets(a, 16, f);
	wndFontDesc.color = atoi(a);
	//fread(&wndFontDesc.bold, sizeof(int), 1, f);
	//fread(&wndFontDesc.italic, sizeof(BOOL), 1, f);
	//fread(&wndFontDesc.underline, sizeof(BOOL), 1, f);
	//fread(&wndFontDesc.color, sizeof(DWORD), 1, f);
	fclose(f);
	
	return OK;
}

RESULT UI_API CreateDesktop()
{
	if(Desktop)
	{
		delete Desktop;
		Desktop = NULL;
	}
	Desktop = new UIDesktop;
	Desktop->mod = mod;

	return OK;
}

RESULT UI_API CreateDWindow(char* WindowTitle, MessageProc* MessageProcedure, HANDLE Parent,
							INT x, INT y, INT cx, INT cy, DWORD BackColor, DWORD TitleColor,
							HANDLE TitleTex, HANDLE BodyTex, HANDLE closeText, HANDLE closeOverText, BOOL Title, Style sty, BOOL hasCloseBttn,
							HANDLE* hWnd)
{
	UIWindow* tmpWnd = NULL;
	UIWindow* tCButton = NULL;
	UIWindow* p = NULL;
	HANDLE c1;
	tmpWnd = new UIWindow;
	if(!tmpWnd)
		return OUTOFMEMORY;
	RESULT r = tmpWnd->Create(WindowTitle, MessageProcedure, x, y, cx, cy, BackColor, TitleColor, TitleTex, BodyTex, Title, sty);
	if(RFAILED(r))
		return E_UNKNOWN;
	
	
	*hWnd = Register(tmpWnd);
	if(MessageProcedure)
	{
		tmpWnd->AllowMessages(TRUE);
		SendObjectMessage(NULL, *hWnd, DM_CREATE, 0, 0, 0);
	}
	Desktop->AttachWindow(*hWnd);
	GenUniqueID(&(tmpWnd->Name), *hWnd);
	OutputLogString(tmpWnd->Name); OutputLogString("\n");
	if(Parent)
	{
		SetChild(Parent, *hWnd);
		GetObjectPointer(mod, Parent, OBJ_WINDOW, (CObject**)&p);
		if(!p) return OK;
		tmpWnd->ZOrder = p->ZOrder+1;
	}
	else
	{
		tmpWnd->ZOrder = 0;
		if(Title | hasCloseBttn)
		{
			tCButton = new UIWindow;
			tCButton->Create(" ",  WindowHidden::CloseButtonProc, cx - 17, -17, 17, 17, 0xffffffff, 0, NULL, closeText, FALSE, None);
			tCButton->extra = (DWORD)closeOverText;
			tCButton->ShowWindow();
			tCButton->ZOrder = 1;
			tCButton->AllowMessages(TRUE);
			c1 = Register(tCButton);
			SetChild(*hWnd, c1);
			Desktop->AttachWindow(c1);
		}
	}
	return OK;
}

RESULT UI_API CreateLabel(int x, int y, int cx, int cy, HANDLE parent, char* text, DWORD color, char* fontname, int size,
						  int bold, BOOL italic, BOOL underline, char* name, HANDLE* hLabel)
{
	UILabel* label = NULL;
	label = new UILabel;
	if(label == NULL)
		return OUTOFMEMORY;
	label->Create(x, y, cx, cy, fontname, size, bold, italic, underline);
	strcpy(label->text, text);
	strcpy(label->Name, name);
	label->Color = color;
	*hLabel = Register(label);
	if(parent)
	{
		SetChild(parent, *hLabel);
	}
	label->AllowMessages(FALSE);
	
	return OK;
}

RESULT UI_API UIVisible(HANDLE wnd, BOOL v)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL) return INVALIDHANDLE;

	if(v) w->ShowWindow();
	else  w->HideWindow();
	return OK;
}

BOOL UI_API UIIsVisible(HANDLE wnd)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL) return INVALIDHANDLE;

	return w->IsVisible();
}

BOOL UI_API UIHasParent(HANDLE wnd)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL) return INVALIDHANDLE;

	return (w->Parent != NULL);
}

RESULT UI_API MoveDWindow(HANDLE wnd, int x, int y, int cx, int cy)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL) return INVALIDHANDLE;

	return w->Move(x, y, cx, cy);
}

RESULT UI_API GetWindowInfo(HANDLE wnd, WNDINFO* info)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL)
		return INVALIDHANDLE;
	
	info->wndPos = w->GetPos();
	info->wndSize = w->GetSize();

	return OK;
}

RESULT UI_API SetTitleTexture(HANDLE wnd, HANDLE text)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL) return INVALIDHANDLE;

	w->SetTexture(text, 0);
	return OK;
}

RESULT UI_API SetBodyTexture(HANDLE wnd, HANDLE text)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL) return INVALIDHANDLE;

	w->SetTexture(text, 1);
	return OK;
}

RESULT UI_API LButtonDown(int x, int y)
{
	if(Desktop->ButtonDown(x, y, 1) == NO_INTERSECTION)
	{
		if(currMouseProc) currMouseProc(1, x, y, TRUE);
	}
	return OK;
}

RESULT UI_API LButtonUp(int x, int y)
{
	if(Desktop->ButtonUp(x, y, 1)  == NO_INTERSECTION)
	{
		if(currMouseProc) currMouseProc(1, x, y, FALSE);
	}
	return OK;
}

RESULT UI_API RButtonDown(int x, int y)
{
	if(Desktop->ButtonDown(x, y, 2) == NO_INTERSECTION)
	{
		if(currMouseProc) currMouseProc(2, x, y, TRUE);
	}
	return OK;
}

RESULT UI_API RButtonUp(int x, int y)
{
	if(Desktop->ButtonUp(x, y, 2) == NO_INTERSECTION)
	{
		if(currMouseProc) currMouseProc(2, x, y, FALSE);
	}
	return OK;
}

VOID UI_API SetKey(char key, BOOL s)
{
	POINT p;
	p = GetMousePos();
	keys[key] = s;
	if(currKeyProc) currKeyProc(key, s);
	if(key == VK_LBUTTON)
	{
		if(s)
		{
			LButtonDown(p.x, p.y);
			Desktop->GrabTitleBar(p.x, p.y, TRUE);
		}
		else
		{
			LButtonUp(p.x, p.y);
			Desktop->GrabTitleBar(p.x, p.y, FALSE);
		}
	}
	if(key == VK_RBUTTON)
	{
		if(s)
		{
			RButtonDown(p.x, p.y);
		}
		else
		{
			RButtonUp(p.x, p.y);
		}
	}
	return;
}

BOOL UI_API GetKey(char key)
{
	return keys[key];
}

RESULT UI_API UIClick(INT x, INT y)
{

	return OK;
}

POINT UI_API GetMousePos()
{
	POINT p;
	p.x = mousepos.x; // - offset.x;
	p.y = mousepos.y; //- offset.y;
	return p;
}

RESULT UI_API SetMousePos(POINT p)
{
	mousepos = p;
	Desktop->MouseMove(p.x, p.y);
	OutputLogString("MMove2\n");
	return OK;
}

RESULT UI_API SetWindowOffset(POINT p)
{
	offset = p;
	return OK;
}

RESULT UI_API UpdateMouse()
{
	POINT mp;
	GetCursorPos(&mp);
	mouseDelta.x = float(mp.x - lastMousePos.x);
	mouseDelta.y = float(mp.y - lastMousePos.y);
	lastMousePos = mp;

	return OK;
}

RESULT UI_API GetFontName(char** font)
{
	if(*font == NULL)
		return INVALIDDATA;
	strcpy(*font, fontname);
	return OK;
}

RESULT UI_API GetWindowFontDesc(DFONTDESC* fdesc)
{
	*fdesc = wndFontDesc;
	return OK;
}

RESULT UI_API UIBeginDraw()
{
	SetRenderState(RS_FOGENABLE, FALSE);
	return OK;
}

RESULT UI_API UIEndDraw()
{
	SetRenderState(RS_FOGENABLE, TRUE);
	return OK;
}

RESULT UI_API SetLabelFormat(HANDLE lbl, DWORD format)
{
	UILabel* l = NULL;
	GetObjectPointer(mod, lbl, OBJ_LABEL, (CObject**)&l);
	if(!l)
		return INVALIDHANDLE;
	l->Format = format;
	return OK;
}

RESULT UI_API SetLabelText(HANDLE lbl, char* text)
{
	UILabel* l = NULL;
	GetObjectPointer(mod, lbl, OBJ_LABEL, (CObject**)&l);
	if(!l)
		return INVALIDHANDLE;
	strcpy(l->text, text);
	return OK;
}

RESULT UI_API CreateButton(char* text, HANDLE parent, MessageProc* msgproc, int x, int y, int cx,
						   int cy, HANDLE bg, DWORD color, DWORD textColor, char* fontface, int size, 
						   int bold, bool italic, bool underline, Style sty, HANDLE* button)
{
	UIButton* tmpbttn = NULL;
	tmpbttn = new UIButton;
	if(tmpbttn == NULL)
		return OUTOFMEMORY;
	tmpbttn->Create(text, parent, msgproc, x, y, cx, cy, bg, color, textColor, fontface, size, bold, italic, underline, sty);
	
	*button = Register(tmpbttn);
	return OK;
}

RESULT UI_API GetBodyTexture(HANDLE wnd, HANDLE* text)
{
	UIWindow* w;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL)
		return INVALIDHANDLE;
	*text = w->GetBodyTexture();
	return OK;
}

RESULT UI_API GetTitleTexture(HANDLE wnd, HANDLE* text)
{
	UIWindow* w;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL)
		return INVALIDHANDLE;
	*text = w->GetTitleTexture();
	return OK;
}

RESULT UI_API SetTitleBarHeight(HANDLE hWnd, INT newTBH)
{
	UIWindow* w;
	GetObjectPointer(mod, hWnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL)
		return INVALIDHANDLE;
	w->SetTBH(newTBH);
	return OK;
}

RESULT UI_API LockedXPosition(HANDLE wnd, BOOL lock)
{
	UIWindow* w;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL)
		return INVALIDHANDLE;
	if(lock)
		w->LockXPosition();
	else
		w->UnlockXPosition();
	return OK;
}

RESULT UI_API CreateScrollBar(int x, int y, int cx, int cy, HANDLE Parent, HANDLE LinkObj, HANDLE BGText, 
							  HANDLE ButtonDownTexture, HANDLE ButtonUpTexture, HANDLE ThumbTexture,
							  DWORD BackColor, long Max, HANDLE* Scrollbar)

{
	UIScrollBar* tmpsb = NULL;
	tmpsb = new UIScrollBar;
	if(tmpsb == NULL)
		return OUTOFMEMORY;
	tmpsb->Create(x, y, cx, cy, Parent, BGText, ButtonUpTexture, ButtonDownTexture, ThumbTexture, BackColor, Max);
	*Scrollbar = tmpsb->me;
	tmpsb->SetLinkObject(LinkObj);
	return OK;
}

RESULT UI_API UIAttachToMouse(HANDLE wnd, int offsetX, int offsetY, BOOL b)
{
	UIWindow* w = NULL;
	GetObjectPointer(mod, wnd, OBJ_WINDOW, (CObject**)&w);
	if(w == NULL)
		return INVALIDHANDLE;
	w->AttachToCursor(offsetX, offsetY, b);
	return OK;
}

RESULT UI_API GetLinkObject(HANDLE wnd, HANDLE* lo)
{
	UIScrollBar* sb = NULL;
	GetObjectPointer(mod, wnd, OBJ_SCROLLBAR, (CObject**)&sb);
	if(sb == NULL)
		return INVALIDHANDLE;

	sb->GetLinkObject(lo);
	return OK;
}

RESULT UI_API GetScrollBarData(HANDLE wnd, SCROLLBARDATA* sbData)
{
	UIScrollBar* sb = NULL;
	GetObjectPointer(mod, wnd, OBJ_SCROLLBAR, (CObject**)&sb);
	if(sb == NULL)
		return INVALIDHANDLE;
	
	sbData->Max = sb->Max();
	sbData->Val = sb->Value();
	return OK;
}

RESULT SetCommonFileFolder(char* dir)
{
	if(!common)
		return OUTOFMEMORY;
	strcpy(common, dir);
	return OK;
}

RESULT GetCommonFileFolder(char** dir)
{
	if(!common)
		return OUTOFMEMORY;
	strcpy(*dir, common);
	return OK;
}

RESULT UI_API CreateDisplayBox(int x, int y, int cx, int cy, HANDLE Parent, char* font, int size, int bold, bool italic,
							   bool underline, char* text, DWORD numLines, DWORD BGColor, DWORD bgScrollBarC, HANDLE sbBG, HANDLE sbUp, HANDLE sbDn,
							   HANDLE sbThumb, bool File, HANDLE* DispBox)
{
	UIDisplayBox* tmpDP = NULL;
	tmpDP = new UIDisplayBox;
	RESULT r;
	r = tmpDP->Create(x, y, cx, cy, Parent, font, size, bold, italic, underline, text, numLines, BGColor, bgScrollBarC, sbBG, sbUp, sbDn, sbThumb, File);
	*DispBox = tmpDP->me;
	return r;
}

RESULT UI_API EnumWindows(WndEnumFunc* func)
{
	return Desktop->EnumWindows(func);
}

RESULT UI_API SetDisplayBoxText(HANDLE DispBox, char* newText)
{
	UIDisplayBox* tmpDP = NULL;
	GetObjectPointer(mod, DispBox, OBJ_DISPLAYBOX, (CObject**)&tmpDP);
	if(tmpDP == NULL)
		return INVALIDHANDLE;
	tmpDP->SetText(newText);
	return OK;
}

RESULT UI_API SetSBMax(HANDLE Obj, long Max)
{
	UIScrollBar* tmpSB = NULL;
	GetObjectPointer(mod, Obj, OBJ_SCROLLBAR, (CObject**)&tmpSB);
	if(tmpSB == NULL)
		return INVALIDHANDLE;
	tmpSB->Max(Max);
	return OK;
}

DELTAPOINT UI_API GetMouseDelta()
{
	return mouseDelta;
}

RESULT UI_API CreateCursor(HANDLE texture)
{
	UICursor* tCur = NULL;
	tCur = new UICursor;
	if(tCur == NULL)
		return OUTOFMEMORY;
	tCur->Create(texture);
	cursor = Register(tCur);
	return OK;
}

RESULT UI_API DrawCursor()
{
	RenderObject(cursor);
	return OK;
}

RESULT UI_API SetKeyProc(UIKeyProc* newProc)
{
	currKeyProc = newProc;
	return OK;
}

RESULT UI_API SetMouseProc(UIMouseProc* newProc)
{
	currMouseProc = newProc;
	return OK;
}
