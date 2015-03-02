#ifndef _SYSDEVICE_H
#define _SYSDEVICE_H

//#include <CObject.h>
//#include <types.h>
#include <SYSD3D.h>
#include <Result.h>

enum API
{
	API_D3D = 0,
	API_OGL = 1,	

	API_NONE = 2,

	API_FORCE_DWORD = 0x79999999,
};

enum PRIM_TYPES
{
	PT_TRILIST = 0,
	PT_TRISTRIP,
	PT_TRIFAN,
	PT_POINTS,
	PT_LINELIST,

	PRIM_TYPES_FORCE_DWORD = 0x79999999,
};

class SYSDevice //: public CObject
{
public:
	SYSDevice();
	~SYSDevice();
	
	RESULT SelectAPI(API choice);
	RESULT Create(HWND hWnd, BOOL Window, int cx, int cy);
	VOID Cleanup();
	VOID Flip();
	VOID Clear(DWORD Color);
	API GetAPI()
	{
		return Selection;
	}

	RESULT SetTexture(DWORD stage, LPDIRECT3DTEXTURE8 texture);
	RESULT DrawPrim(PRIM_TYPES PT, UINT startVert, UINT numPrims);
	RESULT DrawIndexedPrim(PRIM_TYPES PT, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims);
	RESULT BeginScene();
	RESULT EndScene();
	RESULT SetRenderState(DWORD rs, DWORD val);
	SYSD3D* GetD3DDevice()
	{
		return D3DDevice;
	}
 
protected:
	API Selection;
	SYSD3D* D3DDevice;
};

SYSDevice::SYSDevice()
{
	//D3DDevice = NULL;
	Selection = API_NONE;
}

SYSDevice::~SYSDevice()
{

}

RESULT SYSDevice::Create(HWND hWnd, BOOL Window, int cx, int cy)
{
	HRESULT hr;
	switch(Selection)
	{
	case API_D3D:
		D3DDevice = new SYSD3D;
		hr = D3DDevice->CreateDevice(hWnd, Window, cx, cy);
		if(FAILED(hr))
		{
			return E_UNKNOWN;
		}
		return OK;

	default:
		return E_UNKNOWN;
	}
}

VOID SYSDevice::Cleanup()
{
	switch(Selection)
	{
	case API_D3D:
		if(D3DDevice != NULL)
			D3DDevice->Cleanup();
		return;
	default:
		return;
	}
}	
RESULT SYSDevice::SetTexture(DWORD stage, LPDIRECT3DTEXTURE8 texture)
{
	//HRESULT hr;
	switch(Selection)
	{
	case API_D3D:
		//hr = D3DDevice->Device()->SetTexture(stage, texture);
		return OK; //HRtoR(hr);
	
	default:
		return E_UNKNOWN;
	}
	return OK;
}

RESULT SYSDevice::SelectAPI(API choice)
{
	Selection = choice;
	return OK;
}

VOID SYSDevice::Flip()
{
	switch(Selection)
	{
	case API_D3D:
		if(FAILED(D3DDevice->Present(NULL, NULL, NULL, NULL)))
		{
			OutputLogString("Flipping Failed\n");
		}
		return;

	default:
		return;
	}
}

VOID SYSDevice::Clear(DWORD Color)
{
	switch(Selection)
	{
	case API_D3D:
		D3DDevice->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color);
		return;
	
	default:
		return;
	}
}

RESULT SYSDevice::DrawPrim(PRIM_TYPES PT, UINT startVert, UINT numPrims)
{
	switch(Selection)
	{
	case API_D3D:
		{
			switch(PT)
			{
			case PT_TRILIST:
				D3DDevice->Device()->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, numPrims);
				return OK;
				break;
			case PT_TRISTRIP:
				D3DDevice->Device()->DrawPrimitive(D3DPT_TRIANGLESTRIP, startVert, numPrims);
				return OK;
				break;
			case PT_TRIFAN:
				D3DDevice->Device()->DrawPrimitive(D3DPT_TRIANGLEFAN, startVert, numPrims);
				return OK;
				break;
			case PT_POINTS:
				D3DDevice->Device()->DrawPrimitive(D3DPT_POINTLIST, startVert, numPrims);
				return OK;
				break;
			case PT_LINELIST:
				D3DDevice->Device()->DrawPrimitive(D3DPT_LINELIST, startVert, numPrims);
				return OK;
			}
			return E_UNKNOWN;
		}
	case API_OGL:
		return NOTIMPL;

	default:
		return APINONE;
	}
}

RESULT SYSDevice::DrawIndexedPrim(PRIM_TYPES PT, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims)
{
	HRESULT hr;
	switch(Selection)
	{
	case API_D3D:
		{
			switch(PT)
			{
			case PT_TRILIST:
				hr = D3DDevice->Device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, minIndex, startVert, startIndex, numPrims);
				return OK;
				break;
			case PT_TRISTRIP:
				D3DDevice->Device()->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, minIndex, startVert, startIndex, numPrims);
				return OK;
				break;
			case PT_TRIFAN:
				D3DDevice->Device()->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, minIndex, startVert, startIndex, numPrims);
				return OK;
				break;
			case PT_POINTS:
				return OK;
				break;
			case PT_LINELIST:
				return OK;
			}
			return E_UNKNOWN;
		}
	case API_OGL:
		return NOTIMPL;

	default:
		return APINONE;
	}
}

RESULT SYSDevice::BeginScene()
{
	switch(Selection)
	{
	case API_D3D:
		D3DDevice->BeginScene();
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

RESULT SYSDevice::EndScene()
{
	switch(Selection)
	{
	case API_D3D:
		D3DDevice->EndScene();
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

RESULT SYSDevice::SetRenderState(DWORD rs, DWORD val)
{
	switch(Selection)
	{
	case API_D3D:
		return HRtoR(D3DDevice->SetRenderState((D3DRENDERSTATETYPE)rs, val));
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

#endif