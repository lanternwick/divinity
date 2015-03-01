#include "D3DDevice.h"

static DWORD FtoDW(float f)
// utility function
{
	return *(DWORD*)&f;
}

D3DDevice::D3DDevice()
{
	myDevice = NULL;
}

D3DDevice::~D3DDevice()
{
	Cleanup();
}

HWND D3DDevice::hWnd() const
{
	// get the associtated HWND
	return my_hWnd;
}

RESULT D3DDevice::Create(HWND hWnd, BOOL Window, int cx, int cy)
//Create a Device or otherwise initialize the rendering API
{
	LPDIRECT3D8 D3D = NULL;
	ScreenX = cx;
	ScreenY = cy;
	// initialize D3D
	if(NULL == (D3D = Direct3DCreate8(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Device Creation Failed!\nD3D Object creation Failed.\nMake sure you have DirectX 8.1 installed", "", MB_OK);
		return E_UNKNOWN;
	}

	D3DDISPLAYMODE d3ddm;
	// get default adapter values
	if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(0, "Device Creation Failed\nCould Not Query for Device Info.", "", MB_OK);
		return E_UNKNOWN;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = Window;
	// if the device should be windowed
	if(!(Window))
	{
		// set back buffer height and width
		d3dpp.BackBufferWidth = cx;
		d3dpp.BackBufferHeight = cy;
		d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	// set up other device parameters
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;

	// try to create a hardware device
	if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
								D3DCREATE_HARDWARE_VERTEXPROCESSING,
								&d3dpp, &myDevice)))
	{
		// try to create a software device
		if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
								D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								&d3dpp, &myDevice)))
		{
			// cant create anything
			MessageBox(0, "Device Creation Failed!", "", MB_OK);
			return E_UNKNOWN;
		}
	}
	my_hWnd = hWnd;

	// clear states
	for(int i = 0; i < 256; ++i)
		states[i] = 0;
	

	// turn Z Buffer on
	SetRenderState(D3DRS_ZENABLE, TRUE);
	// turn lighting off
	myDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// set up blending modes
	SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
    SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	// set texture modes
	myDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    myDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    myDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	
	
    myDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    myDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    
    myDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    myDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

	// release D3D
	D3D->Release();
	return OK;
}
	

VOID D3DDevice::Clear(DWORD Color)
//clear scene 
{
	myDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Color, 1.0f, 0);
	return;
}

HRESULT D3DDevice::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
//flip scene
{
	HRESULT hr = myDevice->Present(NULL, NULL, NULL, NULL);
	return hr;
}

VOID D3DDevice::Flip()
{
	// flip the scene
	Present(NULL, NULL, NULL, NULL);
	return;
}

VOID D3DDevice::Cleanup()
{
	if(NULL != myDevice)
	{
		// release the device
		myDevice->Release();
	}
}
	
RESULT D3DDevice::SetRenderState(DWORD rs, DWORD val)
// cache renderstate changes
{
	if(states[rs] != val) // if the state is different
	{
		states[rs] = val; // save this state
		// and update
		return HRtoR(myDevice->SetRenderState((D3DRENDERSTATETYPE)rs, val));
	}
	return OK;
}

RESULT D3DDevice::BeginScene()
{
	// being scene
	return HRtoR(myDevice->BeginScene());
}

RESULT D3DDevice::EndScene()
{
	// end scene
	return HRtoR(myDevice->EndScene());
}

RESULT D3DDevice::SetLight(DWORD index, LIGHT* light)
{	
	// set a light with index
	return HRtoR(myDevice->SetLight(index, (const D3DLIGHT8*)light));
}

RESULT D3DDevice::LightEnable(DWORD index, BOOL enabled)
{
	//enable a light at index
	return HRtoR(myDevice->LightEnable(index, enabled));
}

RESULT D3DDevice::DrawIndexedPrim(PRIM_TYPES pt, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims)
// draw an indexed primitive
{
	switch(pt)
	{
	case PT_TRILIST: // draw a triangle list
		myDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, minIndex, startVert, startIndex, numPrims);
		return OK;
		break;
	case PT_TRISTRIP: // draw a triangle strip
		myDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, minIndex, startVert, startIndex, numPrims);
		return OK;
		break;
	case PT_TRIFAN: // draw a triangle fan
		myDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, minIndex, startVert, startIndex, numPrims);
		return OK;
		break;
	case PT_POINTS: // draw points
		return OK;
		break;
	}
	return IMPOSSIBLE;
}

RESULT D3DDevice::DrawPrim(PRIM_TYPES pt, UINT startVert, UINT numPrims)
// draw primitive
{
	switch(pt)
	{
	case PT_TRILIST: // draw triangle list
		myDevice->DrawPrimitive(D3DPT_TRIANGLELIST, startVert, numPrims);
		return OK;
		break;
	case PT_TRISTRIP: // draw triangle strip
		myDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, startVert, numPrims);
		return OK;
		break;
	case PT_TRIFAN: // draw triangle fan
		myDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, startVert, numPrims);
		return OK;
		break;
	case PT_POINTS: // draw points
		myDevice->DrawPrimitive(D3DPT_POINTLIST, startVert, numPrims);
		return OK;
		break;
	case PT_LINELIST: // draw line list
		myDevice->DrawPrimitive(D3DPT_LINELIST, startVert, numPrims);
		return OK;
	}
	return E_UNKNOWN;
}

RESULT D3DDevice::GetTransform(TRANSFORMATION t, MATRIX* m)
// get current transform
{
	DWORD trans;
	switch(t)
	{
	case T_WORLD: // get world transform
		trans = D3DTS_WORLD;
		break;
	case T_VIEW: // get view transform
		trans = D3DTS_VIEW;
		break;
	case T_PROJECTION: // get projection transformation
		trans = D3DTS_PROJECTION;
		break;
	default:
		return INVALID_TRANSFORM;
	}
	// get current transform
	myDevice->GetTransform((D3DTRANSFORMSTATETYPE)trans, (D3DMATRIX*)m);
	return OK;
}

RESULT D3DDevice::SetTransform(TRANSFORMATION t, MATRIX* m)
// set a new transformation matrix
{
	DWORD trans;
	switch(t)
	{
	case T_WORLD: // set world transform
		trans = D3DTS_WORLD;
		break;
	case T_VIEW: // set view transform
		trans = D3DTS_VIEW;
		break;
	case T_PROJECTION: // set projection transform
		trans = D3DTS_PROJECTION;
		break;
	default:
		return INVALID_TRANSFORM;
	}
	// and set the matrix
	myDevice->SetTransform((D3DTRANSFORMSTATETYPE)trans, (D3DMATRIX*)m);
	return OK;
}

RESULT D3DDevice::CreateVertexBuffer(UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF, VOID** p_vertexBuffer)
{
	// create a vertex buffer
	HRESULT hr;
	hr = myDevice->CreateVertexBuffer(numVerts * VertSize, Usage, FVF, D3DPOOL_DEFAULT, (LPDIRECT3DVERTEXBUFFER8*)p_vertexBuffer);
	return OK;
}

RESULT D3DDevice::SetVertexShader(DWORD shader)
{
	// set vertex shader
	return HRtoR(myDevice->SetVertexShader(shader));
}

RESULT D3DDevice::SetVertexBuffer(VOID* p_vertexBuffer, DWORD stride)
{
	// set the vertex buffer for rendering
	return HRtoR(myDevice->SetStreamSource(0, (LPDIRECT3DVERTEXBUFFER8)p_vertexBuffer, stride));
}

RESULT D3DDevice::CreateIndexBuffer(UINT numIndices, DWORD Usage, BYTE type, VOID** p_indexBuffer)
{
	// create an index buffer
	DWORD r_type = 0;
	DWORD sMul = 0;
	switch(type)
	{
	case 16: // if 16 bits 
		r_type = (DWORD)D3DFMT_INDEX16;
		sMul = 2;
		break;
	case 32: // if 32 bits
		r_type = (DWORD)D3DFMT_INDEX32;
		sMul = 4;
		break;
	default:
		return INVALIDDATA;
	}
	// create the buffer
	return HRtoR(myDevice->CreateIndexBuffer(numIndices*sMul, Usage, (D3DFORMAT)r_type, D3DPOOL_DEFAULT, (LPDIRECT3DINDEXBUFFER8*)p_indexBuffer));
}

RESULT D3DDevice::SetIndices(VOID* p_indexBuffer, UINT startIndices)
{
	return HRtoR(myDevice->SetIndices((LPDIRECT3DINDEXBUFFER8)p_indexBuffer, startIndices));
}

RESULT D3DDevice::SetMaterial(MATERIAL* mat)
{
	// set material
	return HRtoR(myDevice->SetMaterial((D3DMATERIAL8*)mat));
}

RESULT D3DDevice::SetNullTexture()
{
	// clear the texture
	return HRtoR(myDevice->SetTexture(0, NULL));
}

RESULT D3DDevice::LoadTexture(const char* filename, VOID** p_texture)
{
	// load normal texture
	return HRtoR(D3DXCreateTextureFromFile(myDevice, filename, (LPDIRECT3DTEXTURE8*)p_texture));
}


RESULT D3DDevice::LoadTextureEx(const char* filename, DWORD transColor, VOID** p_texture)
{
	// load texture with trans color
	return HRtoR(D3DXCreateTextureFromFileEx(myDevice, filename, D3DX_DEFAULT, D3DX_DEFAULT,
											 0, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, 
											 D3DX_FILTER_NONE, D3DX_FILTER_NONE, transColor, NULL, NULL, 
											 (LPDIRECT3DTEXTURE8*)p_texture));
}

RESULT D3DDevice::SetTexture(DWORD stage, VOID* p_texture)
{
	// set the texture
	return HRtoR(myDevice->SetTexture(stage, (LPDIRECT3DTEXTURE8)p_texture));
}