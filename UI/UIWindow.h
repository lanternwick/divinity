#ifndef _UIWINDOW_H
#define _UIWINDOW_H



#include <CObject.h>
#include <Result.h>
#include "ui.h"
#include "..\system\system.h"
#include <types.h>
#include <fvf.h>

namespace WindowHidden
{
	VOID CloseButtonProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		HANDLE p;
		HANDLE extra;
		HANDLE cText;
		GetRootHandle(h, &p);
		switch(msg)
		{
		case DM_CLICK:
			UIVisible(p, FALSE);
			return;
		case DM_MOUSEOVER:
			GetExtraData(h, (DWORD*)&extra);
			GetBodyTexture(h, &cText);
			SetBodyTexture(h, extra);
			SetExtraData(h, (DWORD)cText);
			return;
		case DM_MOUSEOUT:
			GetExtraData(h, (DWORD*)&extra);
			GetBodyTexture(h, &cText);
			SetBodyTexture(h, extra);
			SetExtraData(h, (DWORD)cText);
			return;
		}
		return;
	}

}


class UIWindow : public CObject
{
public:
	UIWindow();
	~UIWindow();

	RESULT Create(char* WindowTitle, MessageProc* MessageProcedure, 
				  INT x, INT y, INT cx, INT cy, DWORD BackColor, DWORD TitleColor,
				  HANDLE TitleTex, HANDLE BodyTex, BOOL tb, Style sty);
	RESULT Render(DWORD& numPrims);
	RESULT Update();
	
	RESULT ShowWindow()
	{
		Visible = TRUE;
		CObject* c = FirstChild;
		UIWindow* w;
		while(c)
		{
			if(c->QueryObject() == OBJ_WINDOW)
			{
				w = (UIWindow*)c;
				w->ShowWindow();
			}
			c = c->Sibling;
		}
		return OK;
	}
	RESULT HideWindow()
	{
		Visible = FALSE;
		CObject* c = FirstChild;
		UIWindow* w;
		while(c)
		{
			if(c->QueryObject() == OBJ_WINDOW)
			{
				w = (UIWindow*)c;
				w->HideWindow();
			}
			c = c->Sibling;
		}
		return OK;
	}
	
	BOOL IsVisible()
	{
		return Visible;
	}

	RESULT Move(int x, int y, int cx, int cy);
	RESULT SetTexture(HANDLE t, int id);
	BOOL PointIntersectWnd(int x, int y);
	BOOL PointIntersectTitle(int x, int y);
	RESULT AttachToCursor(int offX, int offY, BOOL s);
	POINT GetPos();
	SIZE GetSize();
	INT ZOrder;
	BOOL MouseOver()
	{
		return mouseOver;
	}
	VOID MouseOver(BOOL b)
	{
		mouseOver = b;
	}
	POINT GetDrawPos()
	{
		this->Update();
		return myDrawPos;
	}
	HANDLE GetBodyTexture()
	{
		return BodyTexture;
	}
	HANDLE GetTitleTexture()
	{
		return TitleTexture;
	}
	VOID SetTBH(int newTBH)
	{
		tbh = newTBH;
	}
	VOID LockXPosition()
	{
		lockedX = TRUE;
	}
	VOID UnlockXPosition()
	{
		lockedX = FALSE;
	}

protected:
	HANDLE myVB;
	HANDLE myMat;
	HANDLE TitleTexture;
	HANDLE BodyTexture;
	HANDLE titleFont;
	SIZE mySize;
	POINT myPos;
	POINT myDrawPos;
	RECT myRect;
	RECT myTitleBar;
	DWORD myBackColor;
	DWORD myTitleColor;
	char name[120];
	UINT nameSize;
	BOOL Visible;
	BOOL TitleBar;
	BOOL Is3D;
	BOOL lineBorder;
	int tbh;
	POINT offsetToMouse;
	BOOL attachedToMouse;
	BOOL mouseOver;
	BOOL lockedX;
	DFONTDESC fd;
	BOOL dirty;
	VOID SyncRect()
	{
		
		myRect.top = myDrawPos.y;
		myRect.left = myDrawPos.x;
		myRect.bottom = myDrawPos.y + mySize.cy;
		myRect.right = myDrawPos.x + mySize.cx;
		myTitleBar.top = myRect.bottom;
		myTitleBar.left = myRect.left;
		myTitleBar.right = myRect.right;
		myTitleBar.top = myRect.top - tbh;
		myTitleBar.bottom = myRect.top;
		return;
	}

	VOID UpdateVB()
	{
		const int BorderWidth = 5;
		RESULT r = OK;
		WINDOWVERTEX* v = NULL;	
		if(!TitleBar) tbh = 0;
		SelectObject(myVB);
		r = LockVertexBuffer(0, 0, (BYTE**)&v, LOCK_DISCARD);
		if((RFAILED(r)) || (v == NULL))
		{
			//UnlockVertexBuffer();
			return;
		}
			//body
			v[0] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y), 0.2f, 1.0f, myBackColor, 0.0f, 0.0f);
			v[1] = WindowVertex(float(mySize.cx+myDrawPos.x), float(myDrawPos.y), 0.2f, 1.0f, myBackColor, 1.0f, 0.0f);			
			v[2] = WindowVertex(float(mySize.cx+myDrawPos.x), float(mySize.cy+myDrawPos.y), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			v[3] = WindowVertex(float(myDrawPos.x), float(mySize.cy+myDrawPos.y), 0.2f, 1.0f, myBackColor, 0.0f, 1.0f);

			//titlebar
			v[4] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y-tbh), 0.2f, 1.0f, myTitleColor, 0.0f, 0.0f);
			v[5] = WindowVertex(float(mySize.cx+myDrawPos.x), float(myDrawPos.y-tbh), 0.2f, 1.0f, myTitleColor, 1.0f, 0.0f);			
			v[6] = WindowVertex(float(mySize.cx+myDrawPos.x), float(myDrawPos.y), 0.2f, 1.0f, myTitleColor, 1.0f, 1.0f);
			v[7] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y), 0.2f, 1.0f, myTitleColor, 0.0f, 1.0f);

			//left border
			v[8] = WindowVertex(float(myDrawPos.x-BorderWidth), float(myDrawPos.y-tbh), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			v[9] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y-tbh), 0.2f, 1.0f, myBackColor, 1.0f, 0.0f);			
			v[10] = WindowVertex(float(myDrawPos.x), float(mySize.cy+myDrawPos.y), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			v[11] = WindowVertex(float(myDrawPos.x-BorderWidth), float(mySize.cy+myDrawPos.y), 0.2f, 1.0f, 0, 0.0f, 1.0f);

			//top left corner
			v[12] = WindowVertex(float(myDrawPos.x-BorderWidth), float(myDrawPos.y-tbh-BorderWidth), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			v[13] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y-tbh-BorderWidth), 0.2f, 1.0f, 0, 1.0f, 0.0f);			
			v[14] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y-tbh), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			v[15] = WindowVertex(float(myDrawPos.x-BorderWidth), float(myDrawPos.y-tbh), 0.2f, 1.0f, 0, 0.0f, 1.0f);

			//top border
			v[16] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y-tbh-BorderWidth), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			v[17] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y-tbh-BorderWidth), 0.2f, 1.0f, 0, 1.0f, 0.0f);			
			v[18] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y-tbh), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			v[19] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y-tbh), 0.2f, 1.0f, myBackColor, 0.0f, 1.0f);

			//top right corner
			v[20] = WindowVertex(float(myDrawPos.x+mySize.cx+BorderWidth), float(myDrawPos.y-tbh-BorderWidth), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			v[21] = WindowVertex(float(myDrawPos.x+mySize.cx+BorderWidth), float(myDrawPos.y-tbh), 0.2f, 1.0f, 0, 1.0f, 0.0f);			
			v[22] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y-tbh), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			v[23] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y-tbh-BorderWidth), 0.2f, 1.0f, 0, 0.0f, 1.0f);

			//right border
			v[24] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y-tbh), 0.2f, 1.0f, myBackColor, 1.0f, 0.0f);			
			v[25] = WindowVertex(float(myDrawPos.x+mySize.cx+BorderWidth), float(myDrawPos.y-tbh), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			v[26] = WindowVertex(float(myDrawPos.x+mySize.cx+BorderWidth), float(mySize.cy+myDrawPos.y), 0.2f, 1.0f, 0, 0.0f, 1.0f);
			v[27] = WindowVertex(float(myDrawPos.x+mySize.cx), float(mySize.cy+myDrawPos.y), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			 
			//lower right corner
			v[28] = WindowVertex(float(myDrawPos.x+mySize.cx+BorderWidth), float(myDrawPos.y+mySize.cy+BorderWidth), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			v[29] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y+mySize.cy+BorderWidth), 0.2f, 1.0f, 0, 1.0f, 0.0f);			
			v[30] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			v[31] = WindowVertex(float(myDrawPos.x+mySize.cx+BorderWidth), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, 0, 0.0f, 1.0f);

			//bottom border
			v[32] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, myBackColor, 0.0f, 1.0f);
			v[33] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, myBackColor, 1.0f, 1.0f);
			v[34] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y+mySize.cy+BorderWidth), 0.2f, 1.0f, 0, 1.0f, 0.0f);			
			v[35] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y+mySize.cy+BorderWidth), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			
			//bottom left corner
			v[36] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, myBackColor, 1.0f, 0.0f);			
			v[37] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y+mySize.cy+BorderWidth), 0.2f, 1.0f, 0, 1.0f, 1.0f);
			v[38] = WindowVertex(float(myDrawPos.x-BorderWidth), float(myDrawPos.y+mySize.cy+BorderWidth), 0.2f, 1.0f, 0, 0.0f, 1.0f);
			v[39] = WindowVertex(float(myDrawPos.x-BorderWidth), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, 0, 0.0f, 0.0f);
			
			// line border
			v[40] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);
			v[41] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);

			v[42] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);
			v[43] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);

			v[44] = WindowVertex(float(myDrawPos.x+mySize.cx), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);
			v[45] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);

			v[46] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y+mySize.cy), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);
			v[47] = WindowVertex(float(myDrawPos.x), float(myDrawPos.y), 0.2f, 1.0f, 0xff000000, 0.0f, 0.0f);


		UnlockVertexBuffer();
		return;
	}

		
};

UIWindow::UIWindow()
{
	type = OBJ_WINDOW;
	CreateVertexBuffer(&myVB, 48*sizeof(WINDOWVERTEX), sizeof(WINDOWVERTEX), USAGE_WRITEONLY | USAGE_DYNAMIC, WINDOWVERTFVF);
	FirstChild = NULL;
	Sibling = NULL;
	Parent = NULL;
	Visible = FALSE;
	myProc = NULL;
	attachedToMouse = FALSE;
	mouseOver = FALSE;
	tbh = 17;
	lockedX = FALSE;
	Is3D = FALSE;
	lineBorder = FALSE;
}

UIWindow::~UIWindow()
{
	//DeleteObj(myVB);
}

RESULT UIWindow::Create(char* WindowTitle, MessageProc* MessageProcedure, 
						INT x, INT y, INT cx, INT  cy, DWORD BackColor, DWORD TitleColor, HANDLE TitleTex, 
						HANDLE BodyTex, BOOL tb, Style sty)
{
	char* f = new char[128];
	
	GetFontName(&f);
	GetWindowFontDesc(&fd);
	for(char* i = WindowTitle; *i; ++i)
		name[i-WindowTitle] = *i;
	name[i-WindowTitle] = 0;
	nameSize = i - WindowTitle;
	mySize.cx = cx;
	mySize.cy = cy;
	myPos.x = x;
	myPos.y = y;
	myBackColor = BackColor;
	myTitleColor = TitleColor;
	myProc = MessageProcedure;
	BodyTexture = BodyTex;
	TitleTexture = TitleTex;
	TitleBar = tb;
	switch(sty)
	{
	case _3D:
		Is3D = TRUE;
		break;
	case Flat:
		lineBorder = TRUE;
		break;
	}
	LoadFont(f, 16, fd.bold, fd.italic, fd.underline, &titleFont); 
	UpdateVB();
	SyncRect();
	strcpy(Name, name);
	dirty = TRUE;
	return OK;
}

RESULT UIWindow::Render(DWORD& numPrims)
{	
	RECT r;
	this->Update();
	SelectObject(myVB);
	SetRenderState(RS_FOGENABLE, FALSE);
	SetRenderState(RS_ALPHABLENDENABLE, TRUE);
	//SetRenderState(RS_SRCBLEND, BLEND_SRCALPHA);
	SetRenderState(RS_DESTBLEND, BLEND_INVSRCALPHA);
	CObject* c = FirstChild;
	if(Visible)	
	{
		
		SetNullObject(OBJ_TEXTURE);
		SelectObject(BodyTexture);
		DrawTriFan(0, 2);
		SetNullObject(OBJ_TEXTURE);
		SelectObject(TitleTexture);
		DrawTriFan(4, 2);
		SetNullObject(OBJ_TEXTURE);
		numPrims+=4;
		if(Is3D)
		{
			//SetRenderState(RS_ALPHABLENDENABLE, TRUE);
			DrawTriFan(8, 2);
			DrawTriFan(12, 2);
			DrawTriFan(16, 2);
			DrawTriFan(20, 2);
			DrawTriFan(24, 2);
			DrawTriFan(28, 2);
			DrawTriFan(32, 2);
			DrawTriFan(36, 2);
			numPrims+=16;
		}
		if(lineBorder)
		{
			DrawLineList(40, 4);
			numPrims+=4;
		}
		if(TitleBar)
		{
			r = myTitleBar;
			r.left = myTitleBar.left + 5;
			r.top = myTitleBar.top + 1;
			
			SelectFont(titleFont);
			DrawTextOut(name, r, DT_END_ELLIPSIS | DT_NOCLIP, fd.color);
		}
		
		while(c)
		{
			c->Render(numPrims);
			c = c->Sibling;
		}
		
	}
	c = Sibling;
	while(c)
	{
		c->Render(numPrims);
		c = c->Sibling;
	}
	SetRenderState(RS_FOGENABLE, TRUE);
	SetRenderState(RS_ALPHABLENDENABLE, FALSE);
	SetRenderState(RS_DESTBLEND, BLEND_ONE);
	
	return OK;
}

RESULT UIWindow::Update()
{
	POINT p;
	UIWindow* obj = (UIWindow*)Parent;
	CObject::Update();
	//if(dirty)
	{
		SyncRect();
		UpdateVB();
		dirty = FALSE;
	}
	if(attachedToMouse)
	{
		p = GetMousePos();
		p.x -= offsetToMouse.x;
		p.y -= offsetToMouse.y;
		if(Parent)
		{
			p.x -= myDrawPos.x;
			p.y -= myDrawPos.y;
		}
		Move(lockedX ? myPos.x : p.x, p.y, mySize.cx, mySize.cy);
		SendObjectMessage(me, me, DM_WINDOWMOVE, p.x, p.y, 0);
	}
	if(myProc)
	{
		for(int i = 0; i < 5; ++i)
		{
			if(!MsgQueue.empty())
			{
				DMSG msg;
				msg = MsgQueue.pop();
				myProc(me, msg.src, msg.Message, msg.param1, msg.param2, msg.param3);
			}
		}
	}
	
	if(Parent)
	{
		if(Parent->QueryObject() == OBJ_WINDOW)
		{
			if(obj)
			{
				p = obj->GetDrawPos();
				myDrawPos.x = myPos.x + p.x;
				myDrawPos.y = myPos.y + p.y;
			}
		}
	}
	else
	{
		myDrawPos = myPos;
	}
	
	/*WINDOWVERTEX* ptr = NULL;
	RESULT r = SelectObject(myVB);
	if(RFAILED(r))
	{
		return OUTOFMEMORY;
	}
	r = LockVertexBuffer(0, 0, (BYTE**)&ptr, 0);
	if(RFAILED(r))
	{
		return E_UNKNOWN;
	}
	ptr[0] = WindowVertex(float(myPos.x), float(myPos.y), 0.1f, 1.0f, myBackColor, 0.0f, 0.0f);
	ptr[1] = WindowVertex(float(myPos.x + mySize.cx), float(myPos.y), 0.1f, 1.0f, myBackColor, 1.0f, 0.0f);
	ptr[2] = WindowVertex(float(myPos.x + mySize.cx), float(myPos.y + mySize.cy), 0.1f, 1.0f, myBackColor, 1.0f, 1.0f);
	ptr[3] = WindowVertex(float(myPos.x), float(myPos.y + mySize.cy), 0.1f, 1.0f, myBackColor, 0.0f, 1.0f);
	
	UnlockVertexBuffer();*/
	SyncRect();
	return OK;
}

RESULT UIWindow::Move(int x, int y, int cx, int cy)
{
	myPos.x = x;
	myPos.y = y;
	mySize.cx = cx;
	mySize.cy = cy;
	UpdateVB();
	SyncRect();
	dirty = FALSE;
	return OK;
}

RESULT UIWindow::SetTexture(HANDLE t, int id)
{
	switch(id)
	{
	case 0:
		TitleTexture = t;
		return OK;
	case 1:
		BodyTexture = t;
		return OK;
	default:
		return NOTIMPL;
	}
	return IMPOSSIBLE;
}

BOOL UIWindow::PointIntersectWnd(int x, int y)
{
	POINT p;
	p.x = x;
	p.y = y;
	if(PtInRect(&myRect, p))
		return TRUE;
	return FALSE;
}

BOOL UIWindow::PointIntersectTitle(int x, int y)
{
	POINT p;
	p.x = x;
	p.y = y;
	if(PtInRect(&myTitleBar, p))
		return TRUE;
	return FALSE;
}

RESULT UIWindow::AttachToCursor(int offX, int offY, BOOL s)
{
	SyncRect();
	offsetToMouse.x = offX - myDrawPos.x;
	offsetToMouse.y = offY - myDrawPos.y;
	//if(PointIntersectTitle(offX, offY) || PointIntersectWnd(offX, offY))
		attachedToMouse = s;
	return OK;
}

POINT UIWindow::GetPos()
{
	return myPos;
}

SIZE UIWindow::GetSize()
{
	return mySize;
}

#endif
	