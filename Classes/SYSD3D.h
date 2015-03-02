#ifndef _SYSD3D_H
#define _SYSD3D_H

#include <d3d8.h>
#include <d3dx8.h>
#include <types.h>
#include <utils.h>
#include <FVF.h>


class SYSD3D
{
public:
	SYSD3D();

	HWND hWnd() const;
	HRESULT CreateDevice(HWND hWnd,
						 BOOL Window, 
						 int cx,
						 int cy);
	VOID DarkenScene(float fAmount);
	VOID FadeAmount(float NewAmount);
	float FadeAmount();
	VOID Clear(DWORD Targets, DWORD Color);
	HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	VOID Cleanup();
	POINT GetScreenSize();
	VOID Draw3DLine(LVERTEX* Src, LVERTEX* Dest, DWORD Color);
	VOID Draw3DLine(D3DXVECTOR3* Src, D3DXVECTOR3* Dest, DWORD Color);
	VOID Draw2DLine(TLVERTEX* Src, TLVERTEX* Dest, DWORD Color);
	VOID Draw2DLine(D3DXVECTOR3* Src, D3DXVECTOR3* Dest, DWORD Color);
	HRESULT SetRenderState(D3DRENDERSTATETYPE rs, DWORD val);
	HRESULT BeginScene()
	{
		return myDevice->BeginScene();
	}
	HRESULT EndScene()
	{
		return myDevice->EndScene();
	}
	LPDIRECT3DDEVICE8 Device();

protected:

private:
	HWND my_hWnd;
	UINT ScreenX;
	UINT ScreenY;
	float fAmount2;
	LPDIRECT3DDEVICE8 myDevice;
	DWORD states[256];

};


SYSD3D::SYSD3D()
{
	myDevice = NULL;
}

HWND SYSD3D::hWnd() const
{
	return my_hWnd;
}

HRESULT SYSD3D::CreateDevice(HWND hWnd, BOOL Window, int cx, int cy)
//Create a Device or otherwise initialize the rendering API [Zaei]
{
	LPDIRECT3D8 D3D = NULL;
	ScreenX = cx;
	ScreenY = cy;
	if(NULL == (D3D = Direct3DCreate8(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Device Creation Failed!\nD3D Object creation Failed.", "", MB_OK);
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;
	if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(0, "Device Creation Failed\nCould Not Query for Device Info.", "", MB_OK);
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = Window;
	if(!(Window))
	{
		d3dpp.BackBufferWidth = cx;
		d3dpp.BackBufferHeight = cy;
		d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = TRUE;

	if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
								D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								&d3dpp, &myDevice)))
	{
		MessageBox(0, "Device Creation Failed!", "", MB_OK);
		return E_FAIL;
	}
	my_hWnd = hWnd;

	for(int i = 0; i < 256; ++i)
		states[i] = 0;
	

	SetRenderState(D3DRS_ZENABLE, TRUE);
	myDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
    SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//SetRenderState(D3DRS_COLORVERTEX, TRUE);
	SetRenderState(D3DRS_FOGSTART, FtoDW(1.0f));
	SetRenderState(D3DRS_FOGEND, FtoDW(400.0f));
	SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
	//SetRenderState(D3DRS_FOGENABLE, TRUE);
	//SetRenderState(D3DRS_FOGDENSITY, FtoDW(0.66f));
	SetRenderState(D3DRS_FOGCOLOR, 0x00ffffff);

	//SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	//SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(0.00f) );
    SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.00f) );
    SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.00f) );
    SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.00f) );



	//SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);


	Device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    Device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//Device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
	
    Device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    Device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    //Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    //Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);*/
    myDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    myDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
	D3D->Release();
	return S_OK;
}
	
VOID SYSD3D::DarkenScene(float fAmount)
// draws a big quad on top of the scene, blended to fAmount [Zaei]
{
	VOID* VBPts;
	TLVERTEX Verts[4];
	LPDIRECT3DVERTEXBUFFER8 tempVB = NULL;
	DWORD Color = (fAmount<1.0f) ? ((DWORD)(255*fAmount))<<24L : 0xff000000;
	Verts[0] = TLVertex(0.0f, 0.0f, 0.5f, 1.0f, Color);
	Verts[1] = TLVertex(float(ScreenX), 0.0f, 0.5f, 1.0f, Color);
	Verts[2] = TLVertex(0.0f, float(ScreenY), 0.5f, 1.0f, Color);
	Verts[3] = TLVertex(float(ScreenX), float(ScreenY), 0.5f, 1.0f, Color);
	
	myDevice->CreateVertexBuffer(4*sizeof(TLVERTEX), 0, TLVERTFVF, D3DPOOL_DEFAULT, &tempVB);
	tempVB->Lock(0, sizeof(TLVERTEX), (BYTE**)&VBPts, 0);
	memcpy(VBPts, Verts, sizeof(Verts));
	tempVB->Unlock();
	
	SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	myDevice->SetVertexShader(TLVERTFVF);
	myDevice->SetStreamSource(0, tempVB, sizeof(TLVERTEX));
	myDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	tempVB->Release();
	return;
}

VOID SYSD3D::FadeAmount(float NewAmount)
// Doesnt do anything [Zaei]
{
	fAmount2 = 1.0f -  NewAmount;
	return;
}

float SYSD3D::FadeAmount()
// see above comment [Zaei]
{
	return 1.0f -  fAmount2;
}

VOID SYSD3D::Clear(DWORD Targets, DWORD Color)
//clear scene [Zaei]
{
	if(FAILED(myDevice->Clear(0, NULL, Targets, Color, 1.0f, 0)))
	{
		OutputLogString("Clear Failed...\n");
	}
	return;
}

HRESULT SYSD3D::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
//flip scene [Zaei]
{
	HRESULT hr = myDevice->Present(NULL, NULL, NULL, NULL);
	return hr;
}

VOID SYSD3D::Cleanup()
//delete everything [Zaei]
{
	if(NULL != Device)
	{
		myDevice->Release();
	}
}
	
POINT SYSD3D::GetScreenSize()
// This should probably be moved out of this class, dont implement in SYSOGL [Zaei]
{
	POINT temp;
	temp.x = ScreenX;
	temp.y = ScreenY;

	return temp;
}

VOID SYSD3D::Draw3DLine(D3DXVECTOR3* Src, D3DXVECTOR3* Dest, DWORD Color)
// the next few functions do what they say  =)  [Zaei]
{
	LVERTEX temp[2];
	temp[0].x = Src->x;
	temp[0].y = Src->y;
	temp[0].z = Src->z;
	temp[0].color = Color;
	temp[1].x = Dest->x;
	temp[1].y = Dest->y;
	temp[1].z = Dest->z;
	temp[1].color = Color;
	Draw3DLine(&temp[0], &temp[1], Color);
	return;
}

VOID SYSD3D::Draw3DLine(LVERTEX* Src, LVERTEX* Dest, DWORD Color)
{
	LPDIRECT3DVERTEXBUFFER8 VB = NULL;
	LVERTEX* verts;
	D3DXMATRIX id;
	D3DXMatrixIdentity(&id);
	//if(VB == NULL)
	{
		myDevice->CreateVertexBuffer(2*sizeof(LVERTEX), D3DUSAGE_DYNAMIC, LVERTFVF, D3DPOOL_DEFAULT, &VB);
	}
	VB->Lock(0, 0, (BYTE**)&verts, D3DLOCK_DISCARD);
	verts[0] = (*Src);
	verts[0].color = Color;
	verts[1] = (*Dest);
	verts[1].color = Color;
	VB->Unlock();
	myDevice->SetTransform(D3DTS_WORLD, &id);
	myDevice->SetTexture(0, NULL);
	SetRenderState(D3DRS_LIGHTING, FALSE);
	myDevice->SetStreamSource(0, VB, sizeof(LVERTEX));
	myDevice->SetVertexShader(LVERTFVF);
	myDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	SetRenderState(D3DRS_LIGHTING, TRUE);
	return;
}

VOID SYSD3D::Draw2DLine(D3DXVECTOR3* Src, D3DXVECTOR3* Dest, DWORD Color)
{
	TLVERTEX temp[2];
	temp[0].x = Src->x;
	temp[0].y = Src->y;
	temp[0].z = Src->z;
	temp[0].color = Color;
	temp[1].x = Dest->x;
	temp[1].y = Dest->y;
	temp[1].z = Dest->z;
	temp[1].color = Color;
	Draw2DLine(&temp[0], &temp[1], Color);
	return;
}

VOID SYSD3D::Draw2DLine(TLVERTEX* Src, TLVERTEX* Dest, DWORD Color)
{
	LPDIRECT3DVERTEXBUFFER8 VB = NULL;
	TLVERTEX* verts;
	D3DXMATRIX id;
	D3DXMatrixIdentity(&id);
	//if(VB == NULL)
	{
		myDevice->CreateVertexBuffer(2*sizeof(TLVERTEX), D3DUSAGE_DYNAMIC, TLVERTFVF, D3DPOOL_DEFAULT, &VB);
	}
	VB->Lock(0, 0, (BYTE**)&verts, D3DLOCK_DISCARD);
	verts[0] = (*Src);
	verts[0].color = Color;
	verts[1] = (*Dest);
	verts[1].color = Color;
	VB->Unlock();
	myDevice->SetTransform(D3DTS_WORLD, &id);
	SetRenderState(D3DRS_LIGHTING, FALSE);
	myDevice->SetStreamSource(0, VB, sizeof(TLVERTEX));
	myDevice->SetTexture(0, NULL);
	myDevice->SetVertexShader(TLVERTFVF);
	myDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	SetRenderState(D3DRS_LIGHTING, TRUE);
	return;
}

HRESULT SYSD3D::SetRenderState(D3DRENDERSTATETYPE rs, DWORD val)
// in the case of OGL, you will probably have to create a large switch statement.
// see FVF.h for definitons of the various renderstates [Zaei]

//note: you dont have to implement them all at once. [Zaei]
{
	if(states[rs] != val)
	{
		states[rs] = val;
		return myDevice->SetRenderState(rs, val);
	}
	return D3D_OK;
}

LPDIRECT3DDEVICE8 SYSD3D::Device()
{
	return myDevice;
}
#endif



