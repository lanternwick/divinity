// note:  charset font size = 8x11 pixels - 4/20/2002
// note:  charset might be 9x12 pixels 4/23/2002

#ifndef _FONT_H
#define _FONT_H

#include "..\DivGraph\DivGraph.h"
#include "..\Common\types.h"
#include "..\Common\Result.h"

class Font
{
public:
	Font();
	
	~Font();

	RESULT LoadFont(IDevice* device, IGraphObjFactory* factory, char* filename);
	RESULT DrawText(const char* text, RECT text_rect, char* attributes, float scale, DWORD color);
	SIZE GetCellSize();
private:
	ITexture* m_charSet;
	IDevice* m_device;
	IGraphObjFactory* m_factory;
	int cell_h;
	int cell_w;
};

#endif
