#ifndef _UIDESKTOP_H
#define _UIDESKTOP_H

#include <vector>
#include "UIWindow.h"

class UIDesktop
{
public:
	UIDesktop();
	~UIDesktop();

	RESULT ButtonDown(int x, int y, int button);
	RESULT ButtonUp(int x, int y, int button);
	RESULT MouseMove(int x, int y);
	RESULT AttachWindow(HANDLE wnd);
	RESULT GrabTitleBar(int x, int y, BOOL s);
	RESULT EnumWindows(WndEnumFunc* func);
	HMODULE mod;
private:
	std::vector<HANDLE> Windows;
	HANDLE BDown;

};

UIDesktop::UIDesktop()
{
}

UIDesktop::~UIDesktop()
{

}

RESULT UIDesktop::AttachWindow(HANDLE wnd)
{
	Windows.push_back(wnd);
	return OK;
}

RESULT UIDesktop::ButtonDown(int x, int y, int button)
{
	UIWindow* w;
	INT max = 0;
	INT found = -1;
	RESULT r = OK;
	std::vector<UIWindow*> intersections;
	for(int i = 0; i < Windows.size(); ++i)
	{
		r = GetObjectPointer(mod, Windows[i], OBJ_WINDOW, (CObject**)&w);
		if(RFAILED(r))
			continue;
		if(!w->IsVisible())
			continue;
		if((w->PointIntersectWnd(x, y)) || (w->PointIntersectTitle(x, y)))
		{
			intersections.push_back(w);
		}
	}
	for(i = 0; i < intersections.size(); ++i)
	{
		w = intersections[i];
		if(w->ZOrder >= max)
		{
			max = intersections[i]->ZOrder;
			found = i;
		}
	}
	if(found == -1) return NO_INTERSECTION;
	if(w->PointIntersectWnd(x, y))
		SendObjectMessage(NULL, intersections[found]->me, DM_BUTTONDOWN, x, y, button);
	BDown = intersections[found]->me;
	return OK;
}

RESULT UIDesktop::ButtonUp(int x, int y, int button)
{
	UIWindow* w;
	RESULT r = OK;
	INT max = 0;
	INT found = -1;
	std::vector<UIWindow*> intersections;
	for(int i = 0; i < Windows.size(); ++i)
	{
		r = GetObjectPointer(mod, Windows[i], OBJ_WINDOW, (CObject**)&w);
		if(RFAILED(r))
			continue;
		if(!w->IsVisible())
			continue;
		if((w->PointIntersectWnd(x, y)) || (w->PointIntersectTitle(x, y)))
		{
			intersections.push_back(w);
		}
		SendObjectMessage(NULL, w->me, DM_FLUSHMOUSE, x, y, button);
	}
	for(i = 0; i < intersections.size(); ++i)
	{
		if(intersections[i]->ZOrder >= max)
		{
			max = intersections[i]->ZOrder;
			found = i;
		}
	}
	if(found == -1) return NO_INTERSECTION;
	if(w->PointIntersectWnd(x, y))
		SendObjectMessage(NULL, intersections[found]->me, DM_BUTTONUP, x, y, button);
	if(intersections[found]->me == BDown)
	{
		SendObjectMessage(NULL, BDown, DM_CLICK, x, y, button);
	}
	return OK;
}	

RESULT UIDesktop::MouseMove(int x, int y)
{
	UIWindow* w;
	RESULT r = OK;
	INT max = 0;
	INT found = -1;
	std::vector<UIWindow*> intersections;
	for(int i = 0; i < Windows.size(); ++i)
	{
		r = GetObjectPointer(mod, Windows[i], OBJ_WINDOW, (CObject**)&w);
		if(RFAILED(r))
			continue;
		if(!w->IsVisible())
			continue;
		if((w->PointIntersectWnd(x, y)) || (w->PointIntersectTitle(x, y)))
		{
			intersections.push_back(w);
		}
		else
		{
			if(w->MouseOver())
			{
				SendObjectMessage(NULL, w->me, DM_MOUSEOUT, 0, 0, 0);
				w->MouseOver(FALSE);
			}
		}

	}
	for(i = 0; i < intersections.size(); ++i)
	{
		if(intersections[i]->ZOrder >= max)
		{
			max = intersections[i]->ZOrder;
			found = i;
		}
	}
	if(found == -1) return NO_INTERSECTION;
	w = intersections[found];
	if(w->PointIntersectWnd(x, y))
		SendObjectMessage(NULL, w->me, DM_MOUSEMOVE, x - w->GetDrawPos().x, y - w->GetDrawPos().y, 0);
	if(!w->MouseOver())
	{
		SendObjectMessage(NULL, w->me, DM_MOUSEOVER, 0, 0, 0);
		w->MouseOver(TRUE);
	}
	return OK;
}

RESULT UIDesktop::GrabTitleBar(int x, int y, BOOL s)
{
	POINT p;
	UIWindow* w;
	RESULT r = OK;
	std::vector<UIWindow*> intersections;
	for(int i = 0; i < Windows.size(); ++i)
	{
		r = GetObjectPointer(mod, Windows[i], OBJ_WINDOW, (CObject**)&w);
		if(RFAILED(r))
			continue;
		if(!w->IsVisible())
			continue;
		if(w->PointIntersectTitle(x, y))
		{
			intersections.push_back(w);
			p = w->GetPos();
			w->AttachToCursor(x, y, s);
		}
	}
	return OK;
}

RESULT UIDesktop::EnumWindows(WndEnumFunc* func)
{
	UIWindow* w;
	RESULT r = OK;
	HANDLE tmp;
	
	for(int i = 0; i < Windows.size(); ++i)
	{
		tmp = Windows[i];
		r = GetObjectPointer(mod, tmp, OBJ_WINDOW, (CObject**)&w);
		if(RFAILED(r))
			return r;
		if(w->Parent == NULL)
		{
			if(!func(tmp)) break;
		}
	}
	return OK;
}

#endif