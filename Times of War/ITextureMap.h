#ifndef _ITEXTUREMAP_H
#define _ITEXTUREMAP_H

#include "..\DivGraph\ITexture.h"
#include "..\Common\custmoniker.h"

class ITextureMap
{
public:
	ITextureMap(){}
	virtual ~ITextureMap(){}

	virtual moniker GetTexture(unsigned long index)=0;
	virtual void AttachTexture(moniker& texture_name)=0;	
	virtual unsigned long GetNumTextures()=0;
	virtual unsigned char GetTextureID(int x, int z)=0;
	virtual void CreateMap(unsigned long w, unsigned char* buffer)=0;
};

#endif