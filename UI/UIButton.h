#ifndef _UIBUTTON_H
#define _UIBUTTON_H

#include <Cobject.h>

class UIButton : public CObject
{
public:
	UIButton();
	~UIButton();

	RESULT Create(char* Text, HANDLE parent, MessageProc* msgProc, int x, int y, int cx, int cy, 
					HANDLE BGImage, DWORD BackColor, DWORD textColor,  char* fontface, int fontsize, int bold, bool italic, 
					bool underline, Style sty);
	RESULT Move(int x, int y, int cx, int cy);
	POINT GetPos();
	SIZE GetSize();
	RESULT SetText(char* t);

	RESULT Render(DWORD& numPrims);
	RESULT Update();
private:
	char* text;
	POINT myPos;
	SIZE mySize;
	HANDLE myWnd;
	HANDLE myLabel;
};

UIButton::UIButton()
{
	text = NULL;	
}

UIButton::~UIButton()
{
	if(text) delete text;
}

RESULT UIButton::Create(char* Text, HANDLE parent, MessageProc* msgProc, int x, int y, int cx, int cy, 
						HANDLE BGImage, DWORD BackColor, DWORD textColor,  char* fontface, int fontsize, int bold, bool italic, 
						bool underline, Style sty)
{
	text = new char[128];
	strcpy(text, Text);

	myPos.x = x;
	myPos.y = y;
	mySize.cx = cx;
	mySize.cy = cy;

	CreateDWindow(Text, msgProc, parent, x, y, cx, cy, BackColor, 0, NULL, BGImage, NULL, NULL, FALSE, sty, FALSE, &myWnd);
	UIVisible(myWnd, TRUE);
	CreateLabel(0, 0, cx, cy, myWnd, text, textColor, fontface, fontsize, bold, italic, underline, "ButtonLabel", &myLabel);

	SetLabelFormat(myLabel, DT_CENTER | DT_VCENTER);
	return OK;
}

RESULT UIButton::Move(int x, int y, int cx, int cy)
{
	myPos.x = x;
	myPos.y = y;
	mySize.cx = cx;
	mySize.cy = cy;
	return OK;
}
	
POINT UIButton::GetPos()
{
	return myPos;
}

SIZE UIButton::GetSize()
{
	return mySize;
}

RESULT UIButton::SetText(char* t)
{
	strcpy(text, t);
	return OK;
}

RESULT UIButton::Render(DWORD& numPrims)
{
	if(!Parent)
	{
		RenderObject(myWnd);
	}
	return OK;
}

RESULT UIButton::Update()
{
	DMSG msg;
	if(allowsMsgs)
	{
		for(int i = 0; i < 5; ++i)
		{
			if(!MsgQueue.empty())
			{
				msg = MsgQueue.pop();
				myProc(msg.src, me, msg.Message, msg.param1, msg.param2, msg.param3);
			}
		}
	}
	return OK;
}

#endif	