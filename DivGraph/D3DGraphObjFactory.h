#ifndef _D3DGRAPHOBJFACTORY_H
#define _D3DGRAPHOBJFACTORY_H

#include "D3DDevice.h"
#include "IGraphObjFactory.h"
#include "D3DVertexBuffer.h"
#include "D3DIndexBuffer.h"
#include "D3DTexture.h"
#include "..\Common\result.h"

class D3DGraphObjFactory : public IGraphObjFactory
{
public:
	D3DGraphObjFactory();
	~D3DGraphObjFactory();

	VOID SetDevice(CSmartPtr<IDevice> dev);

	RESULT CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, CSmartPtr<IVertexBuffer>& vb, char* key);
	RESULT CreateIndexBuffer(UINT numIndices, DWORD usage, CSmartPtr<IIndexBuffer>& ib, char* key);
	RESULT LoadTexture(char* filename, CSmartPtr<ITexture>& texture, char* key);
	RESULT LoadTextureEx(char* filename, DWORD transColor, CSmartPtr<ITexture>& texture, char* key);
protected:

};

#endif