#ifndef _ITEXTUREMAP_H
#define _ITEXTUREMAP_H

#include "..\DivGraph\ITexture.h"

class ITextureMap
{
public:
	ITextureMap(){}
	virtual ~ITextureMap(){}

	virtual ITexture* GetTexture(unsigned long index)=0;
	virtual void AttachTexture(ITexture* texture, char* texture_name)=0;
	virtual ITexture* GetTextureByTile(unsigned long tileID)=0;
	virtual void AttachTile(unsigned long tileID, char* texture_name)=0;
};

#endif