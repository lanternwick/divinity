#include "D3DTexture.h"
#include "D3DDevice.h"

D3DTexture::D3DTexture()
{
	//type = OBJ_TEXTURE;
	texture = NULL;
	myFilename = new char[128];
	Ex = FALSE;
	myDev = NULL;
	myData = NULL;
	m_refc = 1;
}

D3DTexture::D3DTexture(const char* id)
{	
	texture = NULL;
	myFilename = new char[128];
	Ex = FALSE;
	if(id)
		m_id() = id;
	myDev = NULL;
	myData = NULL;
	m_refc = 1;
}

D3DTexture::~D3DTexture()
{
	// cleanup this texture
	Release();
	if(myFilename != NULL) delete myFilename;
}

/*RESULT D3DTexture::LoadTexture(IDevice* Device, char* Filename)
{
	Core::Function_Enter("D3DTexture::LoadTexture()");
	// load the texture normally
	strcpy(myFilename, Filename);
	myDev = Device;
	Ex = FALSE;
	unsigned long tex_width;
	FILE* f = NULL;
	
	f = fopen(Filename, "r+b");
	if(!f) return FILE_DOES_NOT_EXIST;
	

	fseek(f, 0, SEEK_END);
	tex_width = ftell(f) / 4;
	
	tex_width = sqrt(tex_width);
	fseek(f, 0, SEEK_SET);
	char* buffer = new char[tex_width*tex_width*4];
	fread(buffer, 1, tex_width*tex_width*4, f);
	RESULT r = CreateTextureFromMemory(Device, buffer, tex_width);
	delete buffer;
	Core::Function_Exit();
	return r;
}

RESULT D3DTexture::CreateTextureFromMemory(IDevice* device, char* memory, int width)
{
	LPDIRECT3DTEXTURE8 temp_text = NULL;
	RESULT r = device->CreateTexture(NULL, width, POOL_SYSTEMMEM, (VOID**)&temp_text);
	if(RFAILED(r))
		return r;
	device->RemoveFromResetList(this);
	r = device->CreateTexture(this, width, POOL_DEFAULT, (VOID**)&texture);
	if(RFAILED(r)) 
		return r;
	D3DLOCKED_RECT re;
	temp_text->LockRect(0, &re, NULL, 0);
	unsigned long c = 0;
	char* b = (char*)re.pBits;
	memcpy(b, memory, width*width*4);
	if(!myData)
	{
		myData = new unsigned char[width*width*4];
		memcpy(myData, memory, width*width*4);
	}
	temp_text->UnlockRect(0);
	device->UpdateTexture(temp_text, texture);
	temp_text->Release();
	myDev = device;
	//myFilename = NULL;
	m_width = width;
	return OK;
}

RESULT D3DTexture::LoadTextureEx(IDevice* Device, char* filename, DWORD transColor)
{
	Core::Function_Enter("D3DTexture::LoadTextureEx()");
	// load the texture with a transparent color
	strcpy(myFilename, filename);
	myDev = Device;
	Ex = TRUE;
	myTransColor = transColor;
	Core::Function_Exit();
	return Device->LoadTextureEx(filename, transColor, (VOID**)&texture);
}*/

RESULT D3DTexture::Set(unsigned long stage)
{
	// set the texture for rendering
	if(!texture)
	{
		MakeNull(myDev, stage);
		return INVALIDDATA;
	}
	myDev->SetTexture(stage, (VOID*)texture);
	myDev->SetTextureState(stage, TS_COLOROP, TOP_MODULATE);
	myDev->SetTextureState(stage, TS_COLORARG1, TA_TEXTURE);
	myDev->SetTextureState(stage, TS_COLORARG2, TA_CURRENT);
	return OK;
}

/*RESULT D3DTexture::MakeNull(IDevice* Device, unsigned long stage)
{
	Core::Function_Enter("D3DTexture::MakeNull()");
	RESULT r = Device->SetNullTexture(stage); // kill the texture
	Core::Function_Exit();
	return r;
}*/

VOID D3DTexture::ResetDeviceObjects()
{
	// recreate this texture
	ITexture* t = this;
	myDev->CreateTextureFromMemory(t, m_width, (char*)myData, NULL);
	return;
}

VOID D3DTexture::Release()
{
	if(DecRef()) return;
	if(texture)
	{
		texture->Release();
		
		texture = NULL;
	}
	if(myDev)
		myDev->RemoveFromResetList(this);
	if(myData)
		delete myData;
	myData = NULL;
	delete this;
}

void D3DTexture::Release_SaveData()
{
	if(texture)
	{
		texture->Release();
		
		texture = NULL;
	}
}