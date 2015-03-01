#ifndef _CTEXTURE_H
#define _CTEXTURE_H
#include <assert.h>
#include <d3dx8.h>

#include <CObject.h>
#include <result.h>
#include "SYSDevice.h"

class CTexture : public CObject
{
public:
	CTexture();
	~CTexture();

	RESULT LoadTexture(SYSDevice* Device, char* Filename);
	RESULT LoadTextureEx(SYSDevice* Device, char* filename, DWORD transColor);
	RESULT Set(SYSDevice* Device);
	static RESULT MakeNull(SYSDevice* Device);
	VOID ResetDeviceObjects();
private:
	LPDIRECT3DTEXTURE8 myD3DTexture;
	API myAPI;
	char* myFilename;
	SYSDevice* myDev;
	BOOL Ex;
	DWORD myTransColor;
};

#endif