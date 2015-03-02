#include "TOW_TextureMap.h"
#include "..\DivCore\Core.h"



TOW_TextureMap::TOW_TextureMap()
{
	Core::Function_Enter("TOW_TextureMap::Constructor()");
	
	Core::Function_Exit();
}

TOW_TextureMap::~TOW_TextureMap()
{
	Core::Function_Enter("TOW_TextureMap::Destructor()");
	if(m_map) delete m_map;
	m_map = NULL;
	Core::Function_Exit();
}

void TOW_TextureMap::AttachTexture(moniker& texture_name, int index)
{
	Core::Function_Enter("TOW_TextureMap::AttachTexture()");
	if(index == -1)
	{
		m_texture_store.push_back(texture_name);
	}
	else
	{
		if(index >= m_texture_store.size())
		{
			m_texture_store.resize(index+1);
		}
		m_texture_store[index] = texture_name;
	}
	
	Core::Function_Exit();
}

moniker TOW_TextureMap::GetTexture(unsigned long index)
{
	Core::Function_Enter("TOW_TextureMap::GetTexture()");
	
	Core::Function_Exit();
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