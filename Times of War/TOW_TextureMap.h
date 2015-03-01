#ifndef _TOW_TEXTUREMAP_H
#define _TOW_TEXTUREMAP_H

#include "stdafx.h"
#include "ITextureMap.h"

#include "..\Common\custmoniker.h"
#include <vector>

class TOW_TextureMap : public ITextureMap
{
public:
	TOW_TextureMap();
	~TOW_TextureMap();

	void CreateMap(unsigned long w, unsigned char* buffer);
	moniker GetTexture(unsigned long index);
	void AttachTexture(moniker& texture_name);	
	unsigned long GetNumTextures();
	unsigned char GetTextureID(int x, int z);
private:
	//map<std::string, unsigned long> m_tile_to_texture;
	//map<ITexture*, std::string> m_texture_store;
	std::vector<moniker> m_texture_store;
	unsigned char* m_map;
	unsigned long m_width;
};

#endif