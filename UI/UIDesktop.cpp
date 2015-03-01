
UIDesktop::UIDesktop()
{
}

UIDesktop::~UIDesktop()
{

}

RESULT UIDesktop::AttachWindow(CObject* wnd)
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
		if(Windows[i]->QueryObject() == OBJ_WINDOW)
		{
			w = (UIWindow*)Windows[i];
		}
		else
		{
			continue;
		}
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
		//SendObjectMessage(NULL, intersections[found]->me, DM_BUTTONDOWN, x, y, button);
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
		if(Windows[i]->QueryObject() == OBJ_WINDOW)
		{
			w = (UIWindow*)Windows[i];
		}
		else
		{
			continue;
		}
		if(!w->IsVisible())
			continue;
		if((w->PointIntersectWnd(x, y)) || (w->PointIntersectTitle(x, y)))
		{
			intersections.push_back(w);
		}
		//SendObjectMessage(NULL, w->me, DM_FLUSHMOUSE, x, y, button);
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
		//SendObjectMessage(NULL, intersections[found]->me, DM_BUTTONUP, x, y, button);
	if(intersections[found]->me == BDown)
	{
		//SendObjectMessage(NULL, BDown, DM_CLICK, x, y, button);
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
		if(Windows[i]->QueryObject() == OBJ_WINDOW)
		{
			w = (UIWindow*)Windows[i];
		}
		else
		{
			continue;
		}
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
				//SendObjectMessage(NULL, w->me, DM_MOUSEOUT, 0, 0, 0);
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
		//SendObjectMessage(NULL, w->me, DM_MOUSEMOVE, x - w->GetDrawPos().x, y - w->GetDrawPos().y, 0);
	if(!w->MouseOver())
	{
		//SendObjectMessage(NULL, w->me, DM_MOUSEOVER, 0, 0, 0);
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
		if(Windows[i]->QueryObject() == OBJ_WINDOW)
		{
			w = (UIWindow*)Windows[i];
		}
		else
		{
			continue;
		}
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
		if(Windows[i]->QueryObject() == OBJ_WINDOW)
		{
			w = (UIWindow*)Windows[i];
		}
		else
		{
			continue;
		}
		if(w->Parent == NULL)
		{
			if(!func(tmp)) break;
		}
	}
	return OK;
}

UIDesktop* UIDesktop::_instance = NULL;

UIDesktop* UIDesktop::Instance()
{
	if(_instance == NULL)
	{
		_instance = new UIDesktop;
	}
	return _instance;
}