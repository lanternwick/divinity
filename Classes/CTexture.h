#ifndef _CTEXTURE_H
#define _CTEXTURE_H
#include <assert.h>
#include <d3dx8.h>

#include <CObject.h>
#include <result.h>

class CTexture : public CObject
{
public:
	CTexture();
	~CTexture();

	RESULT LoadTexture(SYSDevice* Device, char* Filename);
	RESULT LoadTextureEx(SYSDevice* Device, char* filename, DWORD transColor);
	RESULT Set(SYSDevice* Device);
	RESULT MakeNull(SYSDevice* Device);
protected:
	LPDIRECT3DTEXTURE8 myD3DTexture;
	API myAPI;
};

CTexture::CTexture()
{
	type = OBJ_TEXTURE;
	myD3DTexture = NULL;
}

CTexture::~CTexture()
{
	if(myD3DTexture != NULL) myD3DTexture->Release();
}

RESULT CTexture::LoadTexture(SYSDevice* Device, char* Filename)
{
	myAPI = Device->GetAPI();
	HRESULT hr;
	OutputLogString(Filename);
	OutputLogString("\n");
	switch(myAPI)
	{
	case API_D3D:
		hr = D3DXCreateTextureFromFile(Device->GetD3DDevice()->Device(), Filename, &myD3DTexture);
		//assert(!hr);
		switch(hr)
		{
		case D3DERR_NOTAVAILABLE:
			MessageBox(0, "NA", "", MB_OK);
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			MessageBox(0, "OVM", "", MB_OK);
			break;
		case D3DERR_INVALIDCALL:
			MessageBox(0, "IC", "", MB_OK);
			
			break;
		case D3DXERR_INVALIDDATA:
			MessageBox(0, "ID", "", MB_OK);
			break;
		case E_OUTOFMEMORY:
			MessageBox(0, "OM", "", MB_OK);
			break;
		case D3D_OK:
			break;
		}
		
		return OK;

	case API_OGL:
		return NOTIMPL;

	default:
		return APINONE;
	}
}

RESULT CTexture::LoadTextureEx(SYSDevice* Device, char* filename, DWORD transColor)
{
	myAPI = Device->GetAPI();
	OutputLogString(filename);
	OutputLogString("\n");
	switch(myAPI)
	{
	case API_D3D:
		D3DXCreateTextureFromFileEx(Device->GetD3DDevice()->Device(), filename, D3DX_DEFAULT, D3DX_DEFAULT,
									D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, 
									D3DX_FILTER_NONE, transColor, NULL, NULL, &myD3DTexture);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
	return IMPOSSIBLE;
}

RESULT CTexture::Set(SYSDevice* Device)
{
	switch(myAPI)
	{
	case API_D3D:
		Device->GetD3DDevice()->Device()->SetTexture(0, myD3DTexture);
		return OK;

	case API_OGL:
		return NOTIMPL;

	default:
		return APINONE;
	}
}

RESULT CTexture::MakeNull(SYSDevice* Device)
{
	switch(myAPI)
	{
	case API_D3D:
		Device->GetD3DDevice()->Device()->SetTexture(0, NULL);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}


#endif