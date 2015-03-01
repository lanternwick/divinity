#include "D3DGraphObjFactory.h"

D3DGraphObjFactory::D3DGraphObjFactory()
{

}

D3DGraphObjFactory::~D3DGraphObjFactory()
{
	myDevice.Release();
}

RESULT D3DGraphObjFactory::CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, CSmartPtr<IVertexBuffer>& vb, char* key)
{
	// create a new vertex buffer
	vb.Release();
	vb.Attach(new D3DVertexBuffer(key));
	if(vb.Null())
		return OUTOFMEMORY;
	vb->Create(myDevice, numVerts, vertSize, usage, FVF);
	Core* c = Core::Instance();
	//c->RegisterMediaObject(vb);
	return OK;
}

RESULT D3DGraphObjFactory::CreateIndexBuffer(UINT numIndices, DWORD usage, CSmartPtr<IIndexBuffer>& ib, char* key)
{
	// create a new index buffer
	ib.Release();
	ib.Attach(new D3DIndexBuffer(key));
	if(ib.Null())
		return OUTOFMEMORY;
	ib->Create(myDevice, numIndices, usage, 16);
	Core* c = Core::Instance();
	//c->RegisterMediaObject(ib);
	return OK;
}

RESULT D3DGraphObjFactory::LoadTexture(char* filename, CSmartPtr<ITexture>& texture, char* key)
{
	// load a texture
	texture.Release();
	texture.Attach(new D3DTexture(key));
	if(texture.Null())
		return OUTOFMEMORY;
	texture->LoadTexture(myDevice, filename);
	Core* c = Core::Instance();
	//c->RegisterMediaObject(texture);
	return OK;
}

RESULT D3DGraphObjFactory::LoadTextureEx(char* filename, DWORD transColor, CSmartPtr<ITexture>& texture, char* key)
{
	// load a texture with a transparent color
	texture.Release();
	texture.Attach(new D3DTexture(key));
	if(texture.Null())
		return OUTOFMEMORY;
	texture->LoadTextureEx(myDevice, filename, transColor);
	Core* c = Core::Instance();
	//c->RegisterMediaObject(texture);
	return OK;
}