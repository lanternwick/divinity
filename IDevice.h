#ifndef IDEVICE_H
#define IDEVICE_H

#include "IMediaObject.h"
//#include "..\Common\IObject.h"
//#include <types.h>
#include "FVF.h"
#include "MATRIX.h"

#include "Result.h"
//#include "..\DivCore\DivCore.h"

#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "ITexture.h"

enum PRIM_TYPES
{
	PT_TRILIST = 0,
	PT_TRISTRIP,
	PT_TRIFAN,
	PT_POINTS,
	PT_LINELIST,
	PT_LINESTRIP,

	PRIM_TYPES_FORCE_DWORD = 0x79999999,
};

enum TRANSFORMATION
{
	T_WORLD = 0,
	T_VIEW,
	T_PROJECTION,

	T_FORCE_DWORD = 0x79999999,
};

class IDevice //: public CObject
{
public:
	IDevice(){};
	virtual ~IDevice(){};
	
	
	virtual RESULT Create(HWND hWnd, BOOL Window, int cx, int cy, DWORD format=0, bool force_format=false)=0;
	virtual RESULT Reset(HWND hWnd, BOOL Window, int cx, int cy, DWORD format=0, bool force_format=false)=0;
	virtual VOID Cleanup()=0;
	virtual VOID Flip()=0;
	virtual VOID Clear(DWORD Color)=0;
	

	
	virtual RESULT DrawPrim(PRIM_TYPES PT, UINT startVert, UINT numPrims)=0;
	virtual RESULT DrawIndexedPrim(PRIM_TYPES PT, UINT minIndex, UINT startVert, UINT startIndex, UINT numPrims)=0;
	virtual RESULT BeginScene()=0;
	virtual RESULT EndScene()=0;
	virtual RESULT SetRenderState(DWORD rs, DWORD val)=0;
	virtual RESULT GetRenderState(DWORD rs, DWORD* val)=0;
	virtual RESULT SetTextureState(DWORD stage, DWORD ts, DWORD val)=0;
	virtual RESULT GetTextureState(DWORD stage, DWORD ts, DWORD* val)=0;
	virtual RESULT SetTransform(TRANSFORMATION t, MATRIX* m)=0;
	virtual RESULT GetTransform(TRANSFORMATION t, MATRIX* m)=0;
	virtual RESULT SetMaterial(MATERIAL* mat)=0;
	virtual RESULT SetLight(DWORD Index, LIGHT* light)=0;
	virtual RESULT LightEnable(DWORD Index, BOOL enabled)=0;

	// creation
	virtual RESULT CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, IVertexBuffer*& vb, const char* key)=0;
	virtual RESULT CreateIndexBuffer(UINT numIndices, DWORD usage, char type, IIndexBuffer*& ib, const char* key)=0;
	virtual RESULT LoadTexture(const char* filename, ITexture*&)=0;
	virtual RESULT CreateTextureFromMemory(ITexture*& texture, int width, char* memory, const char* key)=0;
	virtual RESULT LoadTextureEx(const char* filename, DWORD transColor, ITexture*& texture)=0;

	//virtual RESULT CreateVertexBuffer(IMediaObject* boundObj, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF, VOID** p_vertexBuffer)=0;
	virtual RESULT SetVertexShader(DWORD shader)=0;
	virtual RESULT SetVertexBuffer(VOID* p_vertexBuffer, DWORD stride)=0;
	//virtual RESULT CreateIndexBuffer(IMediaObject* boundObj, UINT numIndices, DWORD Usage, BYTE type, VOID** p_indexBuffer)=0;
	virtual RESULT SetIndices(VOID* p_indexBuffer, UINT startIndices)=0;
	virtual RESULT SetNullTexture(unsigned long stage)=0;
	//virtual RESULT LoadTexture(const char* filename, VOID** p_texture)=0;
	//virtual RESULT CreateTexture(IMediaObject* boundObj, unsigned int width, POOL pool, VOID** p_texture)=0;
	virtual RESULT UpdateTexture(VOID* src, VOID* dest)=0;
	//virtual RESULT LoadTextureEx(const char* filename, DWORD transColor, VOID** p_texture)=0;
	virtual RESULT SetTexture(DWORD stage, VOID* p_texture)=0;

	// utility functions
	
	virtual int width()=0;
	virtual int height()=0;

	virtual void RemoveFromResetList(IMediaObject* obj)=0;

	virtual void BufferDump(char* filename)=0;
protected:
	
	static bool created;
};

#endif