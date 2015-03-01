#include "D3DD3DGraphObjFactory.h"

D3DGraphObjFactory::D3DGraphObjFactory()
{

}

D3DGraphObjFactory::~D3DGraphObjFactory()
{

}

VOID D3DGraphObjFactory::SetDevice(SYSDevice* dev)
{
	myDevice = dev;
	return;
}

RESULT D3DGraphObjFactory::CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, IVertexBuffer** vb)
{
	*vb = NULL;
	*vb = new D3DVertexBuffer;
	if(!(*vb))
		return OUTOFMEMORY;
	(*vb)->Create(myDevice, numVerts, vertSize, usage, FVF);
	return OK;
}

RESULT D3DGraphObjFactory::CreateIndexBuffer(UINT numIndices, DWORD usage, IIndexBuffer** ib)
{
	*ib = NULL;
	*ib = new D3DIndexBuffer;
	if(!(*ib))
		return OUTOFMEMORY;
	(*ib)->Create(myDevice, numIndices, usage);
	return OK;
}

RESULT D3DGraphObjFactory::LoadTexture(char* filename, CTexture** texture)
{
	*texture = NULL;
	*texture = new CTexture;
	if(!(*texture))
		return OUTOFMEMORY;
	(*texture)->LoadTexture(myDevice, filename);
	return OK;
}

RESULT D3DGraphObjFactory::LoadTextureEx(char* filename, DWORD transColor, CTexture** texture)
{
	*texture = NULL;
	*texture = new CTexture;
	if(!(*texture))
		return OUTOFMEMORY;
	(*texture)->LoadTextureEx(myDevice, filename, transColor);
	return OK;
}