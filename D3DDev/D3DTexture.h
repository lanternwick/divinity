#ifndef _D3DTEXTURE_H
#define _D3DTEXTURE_H
#include <assert.h>

#include "..\Common\CObject.h"
#include "..\Common\result.h"
#include "..\DivGraph\ITexture.h"
#include "D3DDevice.h"

class D3DTexture : public ITexture
{
public:
	D3DTexture(char* id);
	virtual ~D3DTexture();

	RESULT LoadTexture(IDevice* Device, char* Filename);
	RESULT CreateTextureFromMemory(IDevice* device, char* memory, int width);
	RESULT LoadTextureEx(IDevice* Device, char* filename, DWORD transColor);
	RESULT Set(unsigned long stage);
	//static RESULT MakeNull(IDevice* Device, unsigned long stage);
	VOID ResetDeviceObjects();
	VOID Release();
	void Release_SaveData();
protected:
	LPDIRECT3DTEXTURE8 texture;
	D3DTexture();
	BYTE* myData;

};

#endif