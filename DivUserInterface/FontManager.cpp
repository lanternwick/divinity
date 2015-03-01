#include "FontManager.h"

FontManager::FontManager()
{

}

FontManager::~FontManager()
{
	for(int i = 0; i < fonts.size(); ++i)
	{
		if(fonts[i].valid)
			RemoveFont(fonts[i].font_name);
	}
}

void FontManager::SetDevice(IDevice* device)
{
	m_device = device;
}

void FontManager::SetFactory(IGraphObjFactory* factory)
{
	m_factory = factory;
}

RESULT FontManager::LoadFont(char* filename, moniker& fontname)
{
	FONT_INFO fi;
	fi.font = new Font;
	RESULT r = fi.font->LoadFont(m_device, m_factory, filename);
	if(RFAILED(r))
	{
		delete fi.font;
		return r;
	}
	fi.font_name() = fontname();
	fi.valid = true;
	fonts.push_back(fi);
	return r;
}

int FontManager::FindFont(const moniker& fontname)
{
	for(int i = 0; i < fonts.size(); ++i)
	{
		if(fonts[i].valid)
		{
			if(fonts[i].font_name() == fontname())
			{
				return i;
			}
		}
	}
	return -1;
}

RESULT FontManager::RemoveFont(const moniker& fontname)
{
	int i = FindFont(fontname);
	if(i == -1) return INVALIDDATA;
	delete fonts[i].font;
	fonts[i].valid = false;
	return OK;
}

RESULT FontManager::DrawText(const moniker& font, const char* text, RECT text_rect, char* attributes, float scale, DWORD color)
{
	int i = FindFont(font);
	if(i == -1) return INVALIDDATA;
	RESULT r = fonts[i].font->DrawText(text, text_rect, attributes, scale, color);
	return r;
}

SIZE FontManager::GetFontCellSize(const moniker& font)
{
	int i = FindFont(font);
	SIZE s = {0, 0};
	if(i == -1) return s;
	return fonts[i].font->GetCellSize();
}