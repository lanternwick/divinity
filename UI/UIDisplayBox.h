#ifndef _UIDISPLAYBOX_H
#define _UIDISPLAYBOX_H

#include <CObject.h>

namespace DBHidden
{
	VOID MainWndProc(HANDLE h, HANDLE src, UINT msg, DWORD p1, DWORD p2, DWORD p3)
	{
		HANDLE db;
		GetExtraData(h, (DWORD*)&db);
		switch(msg)
		{
		case DM_SCROLLUP:
		case DM_SCROLLDN:
			SendObjectMessage(h, db, msg, p1, 0, 0);
			break;
		}
		return;
	}




}


class UIDisplayBox : public CObject
{
public:
	UIDisplayBox();
	~UIDisplayBox();

	RESULT Create(int x, int y, int cx, int cy, HANDLE Parent, char* font, int size, int bold, bool italic, 
				  bool underline, char* Text, DWORD numlines, DWORD BGColor, DWORD bgScrollBarC, HANDLE sbBG, HANDLE sbUp, HANDLE sbDn, HANDLE sbThumb,
				  bool File);

	RESULT Render(DWORD& numPrims);
	RESULT Update();

	RESULT LoadFile(char* filemame);
	VOID SetText(char* newText);
private:
	char* text;
	char* drawtext;
	POINT myPos;
	SIZE mySize;
	DFONTDESC myFD;
	HANDLE MainWnd;
	HANDLE Label;
	HANDLE ScrollBar;
	DWORD numNL;
	DWORD numLines;
	VOID HandleMessages(DMSG m);
	VOID UpdateText(DWORD line);
};

UIDisplayBox::UIDisplayBox()
{
	text = new char[512];
	drawtext = new char[512];
	type = OBJ_DISPLAYBOX;
}

UIDisplayBox::~UIDisplayBox()
{
	delete text;
	delete drawtext;
}

RESULT UIDisplayBox::Create(int x, int y, int cx, int cy, HANDLE Parent, char* font, int size, int bold, bool italic, 
							bool underline, char* Text, DWORD numlines, DWORD BGColor, DWORD bgScrollBarC, HANDLE sbBG, HANDLE sbUp, HANDLE sbDn, HANDLE sbThumb,
							bool File)
{
	myPos.x = x;
	myPos.y = y;
	mySize.cx = cx;
	mySize.cy = cy;
	FILE* f = NULL;
	char* tmp = text;
	int c = 0;
	numNL = 0;
	numLines = numlines;
	if(File)
	{
		f = fopen(Text, "r");
		if(f)
		{
			while((!feof(f)) && (c < 512))
			{
				text[c] = (char)fgetc(f);
				if(text[c] == '\n') ++numNL;
				++c;
			}
		}
		else
		{
			strcpy(text, Text);
			for(int i = 0; i < strlen(text); ++i)
			{
				if(text[i] == '\n') ++numNL;
			}
		}
	}
	else
	{
		strcpy(text, Text);
		for(int i = 0; i < strlen(text); ++i)
		{
			if(text[i] == '\n') ++numNL;
		}
	}
				

	Register(this);
	RESULT r = OK;
	r = CreateDWindow("DispBox", DBHidden::MainWndProc, Parent, x, y, cx, cy, BGColor, 0, NULL, NULL, NULL, NULL, FALSE, Flat, FALSE, &MainWnd);
	if(RFAILED(r))
		return r;
	UIVisible(MainWnd, TRUE);
	SetExtraData(MainWnd, (DWORD)me);
	r = CreateLabel(6, 6, cx, cy, MainWnd, " ", 0xff000000, font, size, bold, italic, underline, "DispBoxLabel", &Label);
	if(RFAILED(r))
		return r;
	UIVisible(Label, TRUE);
	r = CreateScrollBar(cx, 0, 17, cy, MainWnd, MainWnd, sbBG, sbDn, sbUp, sbThumb, bgScrollBarC, numNL - numlines, &ScrollBar);
	if(RFAILED(r))
		return r;
	UIVisible(ScrollBar, TRUE);
	AllowMessages(TRUE);
	UpdateText(0);
	return OK;
}

RESULT UIDisplayBox::Render(DWORD& numPrims)
{
	return RenderObject(MainWnd);
}

RESULT UIDisplayBox::Update()
{
	DMSG msg;
	while(!MsgQueue.empty())
	{
		msg = MsgQueue.pop();
		HandleMessages(msg);
	}
	return OK;
}

RESULT UIDisplayBox::LoadFile(char* filename)
{


	return OK;
}

VOID UIDisplayBox::HandleMessages(DMSG m)
{
	switch(m.Message)
	{
	case DM_SCROLLUP:
	case DM_SCROLLDN:
		UpdateText(m.param1);
		break;
	}
}

VOID UIDisplayBox::SetText(char* newText)
{
	numNL = 0;
	strcpy(text, newText);
	for(int i = 0; i < strlen(text); ++i)
	{
		if(text[i] == '\n') ++numNL;
	}
	long n = numNL - numLines;
	if(n < 0)
	{
		n = 0;
	}
	SetSBMax(ScrollBar, n );
	UpdateText(0);
	return;
}

VOID UIDisplayBox::UpdateText(DWORD line)
{
	DWORD cline = 0;
	DWORD c = 0;
	DWORD drawLen = 0;
	DWORD textLen = strlen(text);
	
	for(UINT i = 0; i < textLen; ++i)
	{
		
		if(text[i] == '\n')
		{
			++cline;
		}
		if(cline == line)
			break;
		++c;
	}
	//++i;
	//if(i == 1) i = 0;
	for(i = 0; i < textLen; ++i)
	{
		
		if(text[i+c] == '\n')
		{
			++cline;
		}
		if(cline == (line+numLines+DWORD(line != 0)))
			break;
		++drawLen;
	}
	memcpy(drawtext, &text[c+DWORD(line != 0)], drawLen-1);
	drawtext[drawLen-1] = NULL;
	SetLabelText(Label, drawtext);
	
	return;
}
		

#endif