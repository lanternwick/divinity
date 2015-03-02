#ifndef _ITEXTUREMAP_H
#define _ITEXTUREMAP_H

#include "ITexture.h"
#include "custmoniker.h"

class ITextureMap
{
public:
	ITextureMap(){}
	virtual ~ITextureMap(){}

	virtual ITexture* GetTexture(unsigned long index)=0;
	virtual void AttachTexture(ITexture* tex, int index=-1)=0;	
	virtual unsigned long GetNumTextures()=0;
	virtual unsigned char GetTextureID(int x, int z)=0;
	virtual void CreateMap(unsigned long w, unsigned char* buffer)=0;
};

#endif