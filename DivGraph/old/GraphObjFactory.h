#ifndef _IGRAPHOBJFACTORY_H
#define _IGRAPHOBJFACTORY_H

#include "SYSDevice.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "CTexture.h"
#include <result.h>

class IGraphObjFactory
{
public:
	IGraphObjFactory();
	virtual ~IGraphObjFactory()=0;

	VOID SetDevice(SYSDevice* dev);

	virtual RESULT CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, IVertexBuffer** vb)=0;
	virtual RESULT CreateIndexBuffer(UINT numIndices, DWORD usage, IIndexBuffer** ib)=0;
	virtual RESULT LoadTexture(char* filename, CTexture** texture)=0;
	virtual RESULT LoadTextureEx(char* filename, DWORD transColor, CTexture** texture)=0;
protected:
	SYSDevice* myDevice;
};

#endif