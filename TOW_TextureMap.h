#ifndef _TOW_TEXTUREMAP_H
#define _TOW_TEXTUREMAP_H

#include "ITextureMap.h"

#include "custmoniker.h"
#include <vector>
#include "ITexture.h"

class TOW_TextureMap : public ITextureMap
{
public:
	TOW_TextureMap();
	~TOW_TextureMap();

	void CreateMap(unsigned long w, unsigned char* buffer);
	ITexture* GetTexture(unsigned long index);
	void AttachTexture(ITexture* tex, int index=-1);	
	unsigned long GetNumTextures();
	unsigned char GetTextureID(int x, int z);
private:
	//map<std::string, unsigned long> m_tile_to_texture;
	//map<ITexture*, std::string> m_texture_store;
	std::vector<ITexture*> m_texture_store;
	unsigned char* m_map;
	unsigned long m_width;
};

#endif