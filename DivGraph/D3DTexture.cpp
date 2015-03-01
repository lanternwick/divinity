#include "D3DTexture.h"

D3DTexture::D3DTexture()
{
	//type = OBJ_TEXTURE;
	texture = NULL;
	myFilename = new char[128];
	Ex = FALSE;
}

D3DTexture::D3DTexture(char* id)
{
	_identifier = NULL;
	_identifier = new char[64];
	strcpy(_identifier, id);
	texture = NULL;
	myFilename = new char[128];
	Ex = FALSE;
}

D3DTexture::~D3DTexture()
{
	// cleanup this texture
	Release();
	if(myFilename != NULL) delete myFilename;
}

RESULT D3DTexture::LoadTexture(CSmartPtr<IDevice> Device, char* Filename)
{
	// load the texture normally
	strcpy(myFilename, Filename);
	myDev = Device;
	Ex = FALSE;
	return Device->LoadTexture(Filename, (VOID**)&texture);
}

RESULT D3DTexture::LoadTextureEx(CSmartPtr<IDevice> Device, char* filename, DWORD transColor)
{
	// load the texture with a transparent color
	strcpy(myFilename, filename);
	myDev = Device;
	Ex = TRUE;
	myTransColor = transColor;
	return Device->LoadTextureEx(filename, transColor, (VOID**)&texture);
}

RESULT D3DTexture::Set(CSmartPtr<IDevice> Device)
{
	// set the texture for rendering
	Device->SetTexture(0, (VOID*)texture);
	return OK;
}

RESULT D3DTexture::MakeNull(CSmartPtr<IDevice> Device)
{
	// kill the texture
	return Device->SetNullTexture();
}

VOID D3DTexture::ResetDeviceObjects()
{
	// recreate this texture
	Release();
	if(Ex)
	{
		LoadTextureEx(myDev, myFilename, myTransColor);
	}
	else
	{
		LoadTexture(myDev, myFilename);
	}
	return;
}

VOID D3DTexture::Release()
{
	if(texture)
	{
		texture->Release();
		texture = NULL;
	}
	myDev.Release();
}