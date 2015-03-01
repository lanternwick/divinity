#ifndef _D3DTEXTURE_H
#define _D3DTEXTURE_H
#include <assert.h>

#include "..\Common\CObject.h"
#include "..\Common\result.h"
#include "ITexture.h"
#include "D3DDevice.h"

class D3DTexture : public ITexture
{
public:
	D3DTexture(char* id);
	~D3DTexture();

	RESULT LoadTexture(CSmartPtr<IDevice> Device, char* Filename);
	RESULT LoadTextureEx(CSmartPtr<IDevice> Device, char* filename, DWORD transColor);
	RESULT Set(CSmartPtr<IDevice> Device);
	static RESULT MakeNull(CSmartPtr<IDevice> Device);
	VOID ResetDeviceObjects();
	VOID Release();
protected:
	LPDIRECT3DTEXTURE8 texture;
	D3DTexture();
};

#endif