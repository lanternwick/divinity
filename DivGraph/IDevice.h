#ifndef _SYSDEVICE_H
#define _SYSDEVICE_H

#include "..\Common\IObject.h"
//#include <types.h>
#include "..\Common\FVF.h"
#include "..\Common\MATRIX.h"

#include "..\Common\Result.h"
#include "..\Common\CSmartPtr.h"


enum PRIM_TYPES
{
	PT_TRILIST = 0,
	PT_TRISTRIP,
	PT_TRIFAN,
	PT_POINTS,
	PT_LINELIST,

	PRIM_TYPES_FORCE_DWORD = 0x79999999,
};

enum TRANSFORMATION
{
	T_WORLD = 0,
	T_VIEW,
	T_PROJECTION,

	T_FORCE_DWORD = 0x79999999,
};

class _API IDevice //: public CObject
{
public:
	IDevice(){};
	virtual ~IDevice(){};
	
	
	virtual RESULT Create(HWND hWnd, BOOL Window, int cx, int cy)=0;
	virtual VOID Cleanup()=0;
	virtual VOID Flip()=0;
	virtual VOID Clear(DWORD Color)=0;
	

	
	virtual RESULT DrawPrim(PRIM_TYPES PT, UINT startVert, UINT numPrims)=0;
	virtual RESULT DrawIndexedPrim(PRIM_TYPES PT, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims)=0;
	virtual RESULT BeginScene()=0;
	virtual RESULT EndScene()=0;
	virtual RESULT SetRenderState(DWORD rs, DWORD val)=0;
	virtual RESULT SetTransform(TRANSFORMATION t, MATRIX* m)=0;
	virtual RESULT GetTransform(TRANSFORMATION t, MATRIX* m)=0;
	virtual RESULT SetMaterial(MATERIAL* mat)=0;
	virtual RESULT SetLight(DWORD Index, LIGHT* light)=0;
	virtual RESULT LightEnable(DWORD Index, BOOL enabled)=0;
	virtual RESULT CreateVertexBuffer(UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF, VOID** p_vertexBuffer)=0;
	virtual RESULT SetVertexShader(DWORD shader)=0;
	virtual RESULT SetVertexBuffer(VOID* p_vertexBuffer, DWORD stride)=0;
	virtual RESULT CreateIndexBuffer(UINT numIndices, DWORD Usage, BYTE type, VOID** p_indexBuffer)=0;
	virtual RESULT SetIndices(VOID* p_indexBuffer, UINT startIndices)=0;
	virtual RESULT SetNullTexture()=0;
	virtual RESULT LoadTexture(const char* filename, VOID** p_texture)=0;
	virtual RESULT LoadTextureEx(const char* filename, DWORD transColor, VOID** p_texture)=0;
	virtual RESULT SetTexture(DWORD stage, VOID* p_texture)=0;
protected:
	
	static bool created;
};

_EXTERN_ template class _API CSmartPtr<IDevice>;

#endif