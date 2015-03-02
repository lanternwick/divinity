#include "ITexture.h"
#include "IDevice.h"

RESULT ITexture::MakeNull(IDevice* Device, unsigned long stage)
{
	for(unsigned long i = stage; i < 8; ++i)
	{
		RESULT r = Device->SetNullTexture(stage); // clear the texture
		Device->SetTextureState(i, TS_COLOROP, TOP_DISABLE);
		
	}	
	return OK;
}

long ITexture::Width()
{
	return m_width;
}

int ITexture::DecRef()
{
	return --m_refc;
}

int ITexture::IncRef()
{
	return ++m_refc;
}