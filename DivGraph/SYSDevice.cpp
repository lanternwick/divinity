
#include "stdafx.h"
#include "SYSDevice.h"
#include <result.h>

bool SYSDevice::created = FALSE;

SYSDevice::SYSDevice()
{
	if(created)
	{
		delete this;
		return;
	}
	created = true;
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

API SYSDevice::GetAPI()
{
	return Selection;
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
			return;
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

RESULT SYSDevice::SetTransform(TRANSFORMATION t, MATRIX* m)
{
	UINT trans = 0;
	switch(Selection)
	{
	case API_D3D:
		switch(t)
		{
		case T_WORLD:
			trans = D3DTS_WORLD;
			break;
		case T_VIEW:
			trans = D3DTS_VIEW;
			break;
		case T_PROJECTION:
			trans = D3DTS_PROJECTION;
			break;
		default:
			return INVALID_TRANSFORM;
		}
		D3DDevice->Device()->SetTransform((D3DTRANSFORMSTATETYPE)trans, (D3DMATRIX*)m);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
	return IMPOSSIBLE;
}

RESULT SYSDevice::SetMaterial(MATERIAL* mat)
{
	D3DDevice->Device()->SetMaterial((D3DMATERIAL8*)mat);
	return OK;
}

RESULT SYSDevice::SetLight(DWORD Index, LIGHT* light)
{
	return HRtoR(D3DDevice->SetLight(Index, light));
}

RESULT SYSDevice::LightEnable(DWORD Index, BOOL enabled)
{
	return HRtoR(D3DDevice->LightEnable(Index, enabled));
}

RESULT SYSDevice::GetTransform(TRANSFORMATION t, MATRIX* m)
{
	UINT trans = 0;
	switch(Selection)
	{
	case API_D3D:
		switch(t)
		{
		case T_WORLD:
			trans = D3DTS_WORLD;
			break;
		case T_VIEW:
			trans = D3DTS_VIEW;
			break;
		case T_PROJECTION:
			trans = D3DTS_PROJECTION;
			break;
		default:
			return INVALID_TRANSFORM;
		}
		D3DDevice->Device()->GetTransform((D3DTRANSFORMSTATETYPE)trans, (D3DMATRIX*)m);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
	return IMPOSSIBLE;
}