#ifndef _FONTMANAGER_H
#define _FONTMANAGER_H

#include <vector>
#include "Font.h"
#include "custmoniker.h"


struct FONT_INFO
{
	moniker font_name;
	Font* font;
	bool valid;
};

class FontManager
{
public:
	FontManager();
	~FontManager();

	void SetDevice(IDevice* device);

	RESULT LoadFont(char* filename, moniker& fontname);
	RESULT RemoveFont(const moniker& fontname);

	RESULT DrawText(const moniker& font, const char* text, RECT text_rect,  char* attributes, float scale, DWORD color);
	SIZE GetFontCellSize(const moniker& font);
	Font* GetFont(const moniker& fontname);
private:
	int FindFont(const moniker& fontname);
	std::vector<FONT_INFO> fonts;
	IDevice* m_device;
};

#endif