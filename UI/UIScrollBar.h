#ifndef _UISCROLLBAR_H
#define _UISCROLLBAR_H

#include <CObject.h>
#include <result.h>
#include "messages.h"
#include <math.h>


namespace ScrollBarHidden
{
	VOID ScrollBarBaseProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		static BOOL dn;
		static POINT pos;
		HANDLE sb;
		GetExtraData(h, (DWORD*)&sb);
		switch(msg)
		{
		case DM_SCROLLUP:
			//SendObjectMessage(h, lo, DM_SCROLLUP, 0, 0, 0);
			SendObjectMessage(h, sb, DM_SCROLLUP, (DWORD)src, 0, 0);
			break;
		case DM_SCROLLDN:
			//SendObjectMessage(h, lo, DM_SCROLLDN, 0, 0, 0);
			SendObjectMessage(h, sb, DM_SCROLLDN, (DWORD)src, 0, 0);
			break;
		case DM_BUTTONDOWN:
			dn = TRUE;
			return;
		case DM_FLUSHMOUSE:
		case DM_BUTTONUP:
			dn = FALSE;
			return;
		case DM_MOUSEMOVE:
			pos.y = p2;
			return;
		case DM_UPDATETICK:
			if(dn)
				SendObjectMessage(h, sb, DM_SCROLL, pos.y, 0, 0);
			return;
		}
	
		return;
	}

	VOID ScrollBarButtonUp(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		static BOOL dn;
		HANDLE p;
		GetParentHandle(h, &p);
		switch(msg)
		{
		case DM_BUTTONDOWN:
			dn = TRUE;
			return;
		case DM_FLUSHMOUSE:
		case DM_BUTTONUP:
			dn = FALSE;
			return;
		case DM_UPDATETICK:
			if(dn)
				SendObjectMessage(h, p, DM_SCROLLUP, 0, 0, 0);
			return;
		}
		return;
	}

	VOID ScrollBarButtonDn(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		static BOOL dn;
		HANDLE p;
		GetParentHandle(h, &p);
		switch(msg)
		{
		case DM_BUTTONDOWN:
			dn = TRUE;
			return;
		case DM_FLUSHMOUSE:
		case DM_BUTTONUP:
			dn = FALSE;
			return;
		case DM_UPDATETICK:
			if(dn)
				SendObjectMessage(h, p, DM_SCROLLDN, 0, 0, 0);
			return;
		}
		return;
	}

	VOID ScrollBarThumb(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		static WNDINFO lastWI;
		WNDINFO wi, wip;
		GetWindowInfo(h, &wi);
		HANDLE p;
		GetParentHandle(h, &p);
		GetWindowInfo(p, &wip);
		POINT po;
		po = GetMousePos();
		switch(msg)
		{
		case DM_BUTTONDOWN:
			//if(msgOff) return;
			if(p3 == 1)
			{
				UIAttachToMouse(h, p1, p2 , TRUE);
				lastWI = wi;
			}
			return;
		case DM_FLUSHMOUSE:
		case DM_BUTTONUP:
			//if(msgOff) return;
			if(p3 == 1)
			{
				UIAttachToMouse(h, 0, 0, FALSE);
			}
			return;
		case DM_WINDOWMOVE:
			//if(msgOff) return;
			
			if(wi.wndPos.y < lastWI.wndPos.y)
				SendObjectMessage(h, p, DM_SCROLLUP, 0, 0, 0);
			if(wi.wndPos.y > lastWI.wndPos.y)
				SendObjectMessage(h, p, DM_SCROLLDN, 0, 0, 0);
		break;
		case DM_UPDATETICK:
			if(wi.wndPos.y < wip.wndSize.cx)
			{
				MoveDWindow(h, wi.wndPos.x, wip.wndSize.cx, wi.wndSize.cx, wi.wndSize.cy);
			}
			if(wi.wndPos.y > wip.wndSize.cy - (wip.wndSize.cx) - wi.wndSize.cy)
			{
				MoveDWindow(h, wi.wndPos.x, wip.wndSize.cy - wi.wndSize.cy - (wi.wndSize.cx), wip.wndSize.cx, wi.wndSize.cy);
			}
			break;
		}
		
		return;
	}

	VOID ScrollBarProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{

	}

}

class UIScrollBar : public CObject
{
public:
	UIScrollBar();
	~UIScrollBar();

	RESULT Create(int x, int y, int cx, int cy, HANDLE Parent, HANDLE BGText, 
				  HANDLE ButtonUpTexture, HANDLE ButtonDownTexture, HANDLE ThumbTexture, 
				  DWORD BackColor, long Max);
	
	RESULT Render(DWORD& numPrims);
	RESULT Update();

	long Max();
	VOID Max(long nMax);

	POINT GetPos();
	SIZE GetSize();

	RESULT GetLinkObject(HANDLE* lObj);
	RESULT SetLinkObject(HANDLE lObj);
	
	long Value();

private:
	long myMax;
	long myVal;

	POINT myPos;
	SIZE mySize;

	HANDLE linkObj;
	HANDLE BodyWnd;
	HANDLE UpButton;
	HANDLE DnButton;
	HANDLE Thumb;
	VOID HandleMessages(DMSG m);
};

UIScrollBar::UIScrollBar()
{
	type = OBJ_SCROLLBAR;
}

UIScrollBar::~UIScrollBar()
{

}

RESULT UIScrollBar::Create(int x, int y, int cx, int cy, HANDLE Parent, HANDLE BGText, 
						   HANDLE ButtonUpTexture, HANDLE ButtonDownTexture, HANDLE ThumbTexture, 
						   DWORD BackColor, long Max)

{
	myPos.x = x;
	myPos.y = y;
	mySize.cx = cx;
	mySize.cy = cy;

	RESULT r;
	Register(this);
	r = CreateDWindow("ScrollbarBase", ScrollBarHidden::ScrollBarBaseProc, Parent, x, y, cx, cy,
						BackColor, 0, NULL, BGText, NULL, NULL, FALSE, Flat, FALSE,  &BodyWnd);
	if(RFAILED(r))
		return r;
	SetExtraData(BodyWnd, (DWORD)me);
	UIVisible(BodyWnd, TRUE);
	r = CreateDWindow("ScrollBarBUp", ScrollBarHidden::ScrollBarButtonUp, BodyWnd, 0, 0, cx, cx, 
		0xffffffff, 0, NULL, ButtonUpTexture, NULL, NULL, FALSE, Flat, FALSE, &UpButton);
	if(RFAILED(r))
		return r;
	UIVisible(UpButton, TRUE);
	r = CreateDWindow("ScrollBarBDn", ScrollBarHidden::ScrollBarButtonDn, BodyWnd, 0, cy-cx, cx, cx, 
		0xffffffff, 0, NULL, ButtonDownTexture, NULL, NULL, FALSE, Flat, FALSE, &DnButton);
	if(RFAILED(r))
		return r;
	UIVisible(DnButton, TRUE);
	r = CreateDWindow("ScrollBarThumb", ScrollBarHidden::ScrollBarThumb, BodyWnd, 0, cx, cx, cy / float(Max), 
		0xffffffff, 0x00ffffff, NULL, ThumbTexture, NULL, NULL, FALSE, Flat, FALSE, &Thumb);
	if(RFAILED(r))
		return r;
	UIVisible(Thumb, TRUE);
	LockedXPosition(Thumb, TRUE);
	AllowMessages(TRUE);
	myVal = 0;
	myMax = Max;
	return OK;
}

RESULT UIScrollBar::Render(DWORD& numPrims)
{
	RenderObject(BodyWnd);
	return OK;
}

RESULT UIScrollBar::Update()
{
	//SetExtraData(BodyWnd, (DWORD)me);
	DMSG m;
	while(!MsgQueue.empty())
	{
		m = MsgQueue.pop();
		HandleMessages(m);
	}

	return OK;
}

VOID UIScrollBar::HandleMessages(DMSG m)
{
	WNDINFO wi, bwi;
	static WNDINFO lastWI;
	float tmpVal;
	GetWindowInfo(Thumb, &wi);
	GetWindowInfo(BodyWnd, &bwi);
	long n;
	BOOL fromT = (m.param1 == (DWORD)Thumb);
	switch(m.Message)
	{
	case DM_SCROLLUP:
		if(fromT) break;	
		if(myVal > 0)
		{
			myVal-=1;
			n = myVal;
			if(n < 0)
				n = 0;
			if(n > myMax)
				n = myMax;
			SendObjectMessage(me, linkObj, DM_SCROLLUP, n, 0, 0);
		}

		break;
	case DM_SCROLLDN:
		if(fromT) break;
		if(myVal < myMax)
		{
			++myVal;
			n = myVal;
			if(n < 0)
				n = 0;
			if(n > myMax)
				n = myMax;
			SendObjectMessage(me, linkObj, DM_SCROLLDN, n, 0, 0);
		}
		break;
	case DM_SCROLL: 
		if(m.param1 < wi.wndPos.y)
		{
			if(myVal > 0)
			{
				myVal-=1;
				n = myVal;
				if(n < 0)
					n = 0;
				if(n > myMax)
					n = myMax;
				SendObjectMessage(me, linkObj, DM_SCROLLUP, n, 0, 0);
			}
		}
		if(m.param1 > wi.wndPos.y+wi.wndSize.cy)
		{
			if(myVal < myMax)
			{
				++myVal;
				n = myVal;
				if(n < 0)
					n = 0;
				if(n > myMax)
					n = myMax;
				SendObjectMessage(me, linkObj, DM_SCROLLDN, n, 0, 0);
			}
		}
		break;
	case DM_UPDATETICK:
		
		break;
	}
	if((m.param1 == (DWORD)Thumb))
	{
		tmpVal = (wi.wndPos.y - wi.wndSize.cx) / float(bwi.wndSize.cy - (2*bwi.wndSize.cx) - wi.wndSize.cy);
		if(tmpVal < 0.0f)
			tmpVal = 0.0f;
		myVal = (long)float(myMax) * tmpVal;
		n = myVal;
		SendObjectMessage(me, linkObj, DM_SCROLLDN, n, 0, 0);
	}
	else
	{
		wi.wndPos.y = wi.wndSize.cx + (myMax == 0 ? 1.0f : (myVal / float(myMax))) * float(bwi.wndSize.cy - wi.wndSize.cy/* - wi.wndSize.cy*/);
		MoveDWindow(Thumb, wi.wndPos.x, wi.wndPos.y, wi.wndSize.cx, (bwi.wndSize.cy - (2*wi.wndSize.cx)) / (float)(myMax == 0 ? 1 : myMax));
	}
	return;
}

long UIScrollBar::Max()
{
	return myMax;
}

VOID UIScrollBar::Max(long nMax)
{
	myMax = nMax;
}

POINT UIScrollBar::GetPos()
{
	return myPos;
}

SIZE UIScrollBar::GetSize()
{
	return mySize;
}

RESULT UIScrollBar::GetLinkObject(HANDLE* lObj)
{
	*lObj = linkObj;
	return OK;
}

RESULT UIScrollBar::SetLinkObject(HANDLE lObj)
{
	linkObj = lObj;
	return OK;
}

long UIScrollBar::Value()
{
	return myVal;
}
#endif