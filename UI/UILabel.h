#ifndef _UILABEL_H
#define _UILABEL_H

#include <CObject.h>

class UILabel : public CObject
{
public:
	UILabel();
	~UILabel();

	char* text;

	RESULT Render(DWORD& numPrims);
	RESULT Update();
	RESULT Create(int x, int y, int cx, int cy, char* fontname, int size, int bold, BOOL italic, BOOL underline);
	RESULT Move(int x, int y, int cx, int cy);
	POINT GetPos();
	SIZE GetSize();
	DWORD Color;
	DWORD Format;
private:
	DFONTDESC fd;
	HANDLE myfont;
	POINT myPos;
	SIZE mySize;
	RECT myDrawRect;
	
	
};

UILabel::UILabel()
{
	text = NULL;
	Format = 0;
	type = OBJ_LABEL;
}

UILabel::~UILabel()
{
	delete text;
}

RESULT UILabel::Render(DWORD& numPrims)
{
	this->Update();
	UIBeginDraw();
	SelectFont(myfont);
	DrawTextOut(text, myDrawRect, Format, Color);
	UIBeginDraw();
	return OK;
}

RESULT UILabel::Update()
{
	UIWindow* u;
	POINT p;
	if(Parent)
	{
		if(Parent->QueryObject() == OBJ_WINDOW)
		{
			u = (UIWindow*)Parent;
			p = u->GetDrawPos();
			myDrawRect.left = p.x + myPos.x;
			myDrawRect.right = p.x + myPos.x + mySize.cx;
			myDrawRect.top = p.y + myPos.y;
			myDrawRect.bottom = p.y + myPos.y + mySize.cy;
		}
	}
	else
	{
		myDrawRect.left = myPos.x;
		myDrawRect.right = myPos.x + mySize.cx;
		myDrawRect.top = myPos.y;
		myDrawRect.bottom = myPos.y + mySize.cy;
	}
	//GetWindowFontDesc(&fd);
	return OK;
}

RESULT UILabel::Create(int x, int y, int cx, int cy, char* fontname, int size, int bold, BOOL italic, BOOL underline)
{
	RESULT r = OK;
	r = LoadFont(fontname, size, bold, italic, underline, &myfont);
	text = new char[512];
	myPos.x = x;
	myPos.y = y;
	mySize.cx = cx;
	mySize.cy = cy;
	return OK;
}

#endif


