#include "ITexture.h"

RESULT ITexture::MakeNull(IDevice* Device, unsigned long stage)
{
	Core::Function_Enter("ITexture::MakeNull()");
	for(unsigned long i = stage; i < 8; ++i)
	{
		RESULT r = Device->SetNullTexture(stage); // clear the texture
		Device->SetTextureState(i, TS_COLOROP, TOP_DISABLE);
		
	}	
	Core::Function_Exit();
	return OK;
}

long ITexture::Width()
{
	return m_width;
}