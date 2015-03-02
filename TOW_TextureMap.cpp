#include "TOW_TextureMap.h"




TOW_TextureMap::TOW_TextureMap()
{
}

TOW_TextureMap::~TOW_TextureMap()
{

	if(m_map) delete m_map;
	m_map = NULL;
}

void TOW_TextureMap::AttachTexture(ITexture* tex, int index)
{
	
	if(index == -1)
	{
		m_texture_store.push_back(tex);
	}
	else
	{
		if(index >= m_texture_store.size())
		{
			m_texture_store.resize(index+1);
		}
		m_texture_store[index] = tex;
	}
	
}

ITexture* TOW_TextureMap::GetTexture(unsigned long index)
{
	
	return m_texture_store[index];
}


unsigned long TOW_TextureMap::GetNumTextures()
{
	return m_texture_store.size();
}

unsigned char TOW_TextureMap::GetTextureID(int x, int z)
{
	if((x >= 0) && (x < m_width) && (z >= 0) && (z < m_width))
		return m_map[z*m_width+x];
	return 0;
}

void TOW_TextureMap::CreateMap(unsigned long w, unsigned char* buffer)
{
	m_width = w;
	m_map = buffer;
}