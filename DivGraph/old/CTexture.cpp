#include "CTexture.h"

CTexture::CTexture()
{
	type = OBJ_TEXTURE;
	myD3DTexture = NULL;
	myFilename = new char[128];
	Ex = FALSE;
}

CTexture::~CTexture()
{
	if(myD3DTexture != NULL) myD3DTexture->Release();
	if(myFilename != NULL) delete myFilename;
}

RESULT CTexture::LoadTexture(SYSDevice* Device, char* Filename)
{
	strcpy(myFilename, Filename);
	myDev = Device;
	Ex = FALSE;
	myAPI = Device->GetAPI();
	HRESULT hr;
	switch(myAPI)
	{
	case API_D3D:
		hr = D3DXCreateTextureFromFile(Device->GetD3DDevice()->Device(), Filename, &myD3DTexture);
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
	strcpy(myFilename, filename);
	myDev = Device;
	Ex = FALSE;
	myTransColor = transColor;
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
	switch(Device->GetAPI())
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

VOID CTexture::ResetDeviceObjects()
{
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