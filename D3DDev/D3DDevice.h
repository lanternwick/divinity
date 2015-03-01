#ifndef _SYSD3D_H
#define _SYSD3D_H

#include <d3d8.h>
#include <d3dx8.h>
#include "..\DivGraph\IDevice.h"
//#include <types.h>
//#include <utils.h>
#include "..\Common\FVF.h"
#include <vector>
#include <algorithm>


class _API D3DDevice : public IDevice
{
public:
	D3DDevice();
	~D3DDevice();
	HWND hWnd() const;
	RESULT Create(HWND hWnd,
				  BOOL Window, 
				  int cx,
				  int cy,
				  DWORD format=0,
				  bool force_format=false);
	RESULT Reset(HWND hWnd,
				 BOOL Window, 
				 int cx,
				 int cy,
				 DWORD format=0,
				 bool force_format=false);
	VOID Clear(DWORD color);
	HRESULT Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	VOID Flip();
	VOID Cleanup();
	POINT GetScreenSize();
	RESULT BeginScene();
	RESULT EndScene();
	RESULT DrawIndexedPrim(enum PRIM_TYPES pt, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims);
	RESULT DrawPrim(enum PRIM_TYPES pt, UINT startVert, UINT numPrims);
	RESULT GetTransform(TRANSFORMATION t, MATRIX* m);
	RESULT LightEnable(DWORD index, BOOL enable);
	RESULT SetLight(DWORD index, LIGHT* l);
	RESULT SetMaterial(MATERIAL* mat);
	RESULT SetRenderState(DWORD rs, DWORD val);
	RESULT GetRenderState(DWORD rs, DWORD* val);
	RESULT SetTextureState(DWORD stage, DWORD ts, DWORD val);
	RESULT GetTextureState(DWORD stage, DWORD ts, DWORD* val);
	RESULT SetTransform(TRANSFORMATION t, MATRIX* m);
	RESULT CreateVertexBuffer(IMediaObject* boundObj, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF, VOID** p_vertexBuffer);
	RESULT SetVertexShader(DWORD shader);
	RESULT SetVertexBuffer(VOID* p_vertexBuffer, DWORD stride);
	RESULT CreateIndexBuffer(IMediaObject* boundObj, UINT numIndices, DWORD Usage, BYTE type, VOID** p_indexBuffer);
	RESULT SetIndices(VOID* p_indexBuffer, UINT startIndices);
	RESULT SetNullTexture(unsigned long stage);
	RESULT LoadTexture(const char* filename, VOID** p_texture);
	RESULT CreateTexture(IMediaObject* boundObj, unsigned int width, POOL pool, VOID** p_texture);
	RESULT UpdateTexture(VOID* src, VOID* dest);
	RESULT LoadTextureEx(const char* filename, DWORD transColor, VOID** p_texture);
	RESULT SetTexture(DWORD stage, VOID* p_texture);

	VECTOR* Unproject(VECTOR* r, VECTOR* src, float fAspect);
	VECTOR* Project(VECTOR* r, VECTOR* src);
	ray* ComputeRay(ray* r, VECTOR* s, CCamera* cam);
	int width();
	int height();
	void RemoveFromResetList(IMediaObject* obj);
protected:

private:
	HWND my_hWnd;
	UINT ScreenX;
	UINT ScreenY;
	D3DDISPLAYMODE d3ddm;
	LPDIRECT3DDEVICE8 myDevice;
	DWORD states[256];
	DWORD tstates[8][28];
	std::vector<IMediaObject*> reset_objects;
	typedef std::vector<IMediaObject*>::iterator rset_iter;

};

#endif



