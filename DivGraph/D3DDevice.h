#ifndef _SYSD3D_H
#define _SYSD3D_H

#include <d3d8.h>
#include <d3dx8.h>
#include "IDevice.h"
//#include <types.h>
//#include <utils.h>
#include "..\Common\FVF.h"

class _API D3DDevice : public IDevice
{
public:
	D3DDevice();
	~D3DDevice();
	HWND hWnd() const;
	RESULT Create(HWND hWnd,
				  BOOL Window, 
				  int cx,
				  int cy);
	/*VOID DarkenScene(float fAmount);
	VOID FadeAmount(float NewAmount);
	float FadeAmount();*/
	VOID Clear(DWORD color);
	HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	VOID Flip();
	VOID Cleanup();
	POINT GetScreenSize();
	/*VOID Draw3DLine(LVERTEX* Src, LVERTEX* Dest, DWORD Color);
	VOID Draw3DLine(D3DXVECTOR3* Src, D3DXVECTOR3* Dest, DWORD Color);
	VOID Draw2DLine(TLVERTEX* Src, TLVERTEX* Dest, DWORD Color);
	VOID Draw2DLine(D3DXVECTOR3* Src, D3DXVECTOR3* Dest, DWORD Color);*/
	RESULT BeginScene();
	RESULT EndScene();
	RESULT DrawIndexedPrim(enum PRIM_TYPES pt, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims);
	RESULT DrawPrim(enum PRIM_TYPES pt, UINT startVert, UINT numPrims);
	RESULT GetTransform(TRANSFORMATION t, MATRIX* m);
	RESULT LightEnable(DWORD index, BOOL enable);
	RESULT SetLight(DWORD index, LIGHT* l);
	RESULT SetMaterial(MATERIAL* mat);
	RESULT SetRenderState(DWORD rs, DWORD val);
	RESULT SetTransform(TRANSFORMATION t, MATRIX* m);
	RESULT CreateVertexBuffer(UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF, VOID** p_vertexBuffer);
	RESULT SetVertexShader(DWORD shader);
	RESULT SetVertexBuffer(VOID* p_vertexBuffer, DWORD stride);
	RESULT CreateIndexBuffer(UINT numIndices, DWORD Usage, BYTE type, VOID** p_indexBuffer);
	RESULT SetIndices(VOID* p_indexBuffer, UINT startIndices);
	RESULT SetNullTexture();
	RESULT LoadTexture(const char* filename, VOID** p_texture);
	RESULT LoadTextureEx(const char* filename, DWORD transColor, VOID** p_texture);
	RESULT SetTexture(DWORD stage, VOID* p_texture);
protected:

private:
	HWND my_hWnd;
	UINT ScreenX;
	UINT ScreenY;
	LPDIRECT3DDEVICE8 myDevice;
	DWORD states[256];

};

#endif



