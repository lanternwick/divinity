#ifndef _IGRAPHOBJFACTORY_H
#define _IGRAPHOBJFACTORY_H

#include "IDevice.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "ITexture.h"
#include "..\Common\result.h"
#include "..\Common\CSmartPtr.h"

class IGraphObjFactory
{
public:
	IGraphObjFactory(){};
	virtual ~IGraphObjFactory(){};

	VOID SetDevice(CSmartPtr<IDevice> dev);

	virtual RESULT CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, CSmartPtr<IVertexBuffer>& vb, char* key)=0;
	virtual RESULT CreateIndexBuffer(UINT numIndices, DWORD usage, CSmartPtr<IIndexBuffer>& ib, char* key)=0;
	virtual RESULT LoadTexture(char* filename, CSmartPtr<ITexture>&, char* key)=0;
	virtual RESULT LoadTextureEx(char* filename, DWORD transColor, CSmartPtr<ITexture>& texture, char* key)=0;
protected:
	CSmartPtr<IDevice> myDevice;
};

_EXTERN_ template class _API CSmartPtr<IGraphObjFactory>;

#endif