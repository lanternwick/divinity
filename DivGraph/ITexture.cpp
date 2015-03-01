#include "ITexture.h"

RESULT ITexture::MakeNull(CSmartPtr<IDevice> Device)
{
	// clear the texture
	return Device->SetNullTexture();
}