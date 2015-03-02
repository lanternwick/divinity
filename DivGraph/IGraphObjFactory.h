#ifndef _IGRAPHOBJFACTORY_H
#define _IGRAPHOBJFACTORY_H

#include "IDevice.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "ITexture.h"
#include "..\Common\result.h"


class _API IGraphObjFactory
{
public:
	IGraphObjFactory(){};
	virtual ~IGraphObjFactory(){};

	VOID SetDevice(IDevice* dev);

	virtual RESULT CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, IVertexBuffer*& vb, char* key)=0;
	virtual RESULT CreateIndexBuffer(UINT numIndices, DWORD usage, IIndexBuffer*& ib, char* key)=0;
	virtual RESULT LoadTexture(char* filename, ITexture*&, char* key)=0;
	virtual RESULT CreateTextureFromMemory(ITexture*& texture, int width, char* memory, char* key)=0;
	virtual RESULT LoadTextureEx(char* filename, DWORD transColor, ITexture*& texture, char* key)=0;
protected:
	IDevice* myDevice;
};


#endif