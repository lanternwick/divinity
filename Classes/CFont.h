#ifndef _CFONT_H
#define _CFONT_H


#include <Cobject.h>


class CFont : public CObject
{
public:
	CFont();
	~CFont();

	RESULT LoadFont(SYSDevice* Device, char* fontFace, INT size, INT bold, BOOL italic, BOOL underline);
	RESULT DrawTextOut(char* text, RECT fontRect, DWORD format, DWORD color);
private:
	LPD3DXFONT myD3DFont;
	HFONT myFontDesc;
};

CFont::CFont()
{
	type = OBJ_DFONT;
}

CFont::~CFont()
{
	myD3DFont->Release();
}

RESULT CFont::LoadFont(SYSDevice* Device, char* fontFace, INT size, INT bold, BOOL italic, BOOL underline)
{
	HRESULT hr;
	myFontDesc = CreateFontA(size, 0, 0, 0, 700*bold, italic, underline, FALSE, DEFAULT_CHARSET, 
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
						DEFAULT_PITCH | FF_DONTCARE, fontFace);
	hr = D3DXCreateFont(Device->GetD3DDevice()->Device(), myFontDesc, &myD3DFont);
	if(FAILED(hr))
	{
		return HRtoR(hr);
	}

	return OK;
}

RESULT CFont::DrawTextOut(char* text, RECT fontRect, DWORD format, DWORD color)
{
	myD3DFont->Begin();
	myD3DFont->DrawText(text, -1, &fontRect, format, color);
	myD3DFont->End();
	return OK;
}

#endif