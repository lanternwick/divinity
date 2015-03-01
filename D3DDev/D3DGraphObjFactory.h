#ifndef _D3DGRAPHOBJFACTORY_H
#define _D3DGRAPHOBJFACTORY_H

#include "D3DDevice.h"
#include "..\DivGraph\IGraphObjFactory.h"
#include "D3DVertexBuffer.h"
#include "D3DIndexBuffer.h"
#include "D3DTexture.h"
#include "..\Common\result.h"

class _API D3DGraphObjFactory : public IGraphObjFactory
{
public:
	D3DGraphObjFactory();
	virtual ~D3DGraphObjFactory();

	VOID SetDevice(IDevice* dev);

	RESULT CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, IVertexBuffer*& vb, char* key);
	RESULT CreateIndexBuffer(UINT numIndices, DWORD usage, IIndexBuffer*& ib, char* key);
	RESULT LoadTexture(char* filename, ITexture*& texture, char* key);
	RESULT CreateTextureFromMemory(ITexture*& texture, int width, char* memory, char* key);
	RESULT LoadTextureEx(char* filename, DWORD transColor, ITexture*& texture, char* key);
protected:

};

#endif