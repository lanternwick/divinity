#include "D3DGraphObjFactory.h"

D3DGraphObjFactory::D3DGraphObjFactory()
{
	Core::Function_Enter("D3DGraphObjFactory::Constructor()");
	Core::Function_Exit();
}

D3DGraphObjFactory::~D3DGraphObjFactory()
{
	Core::Function_Enter("D3DGraphObjFactory::Destructor()");
	Core::Function_Exit();
}

RESULT D3DGraphObjFactory::CreateVertexBuffer(UINT numVerts, UINT vertSize, DWORD usage, DWORD FVF, IVertexBuffer*& vb, char* key)
{
	// create a new vertex buffer
	Core::Function_Enter("D3DGraphObjFactory::CreateVertexBuffer()");
	vb = new D3DVertexBuffer(key);
	if(!vb)
	{
		Core::Function_Exit();
		return OUTOFMEMORY;
	}
	Core* c = Core::Instance();
	
	if((!key) || (c->RegisterMediaObject(vb)))
		vb->Create(myDevice, numVerts, vertSize, usage, FVF);
	
	Core::Function_Exit();
	return OK;
}

RESULT D3DGraphObjFactory::CreateIndexBuffer(UINT numIndices, DWORD usage, IIndexBuffer*& ib, char* key)
{
	// create a new index buffer
	Core::Function_Enter("D3DGraphObjFactory::CreateIndexBuffer()");
	ib = new D3DIndexBuffer(key);
	if(!ib)
		return OUTOFMEMORY;
	Core* c = Core::Instance();
	if((!key) || (c->RegisterMediaObject(ib)))
		ib->Create(myDevice, numIndices, usage, 16);
	
	Core::Function_Exit();
	return OK;
}

RESULT D3DGraphObjFactory::LoadTexture(char* filename, ITexture*& texture, char* key)
{
	// load a texture
	Core::Function_Enter("D3DGraphObjFactory::LoadTexture()");
	texture = new D3DTexture(key);
	if(!texture)
		return OUTOFMEMORY;
	Core* c = Core::Instance();
	log_stream<1> ls;
	if((!key) || (c->RegisterMediaObject(texture)))
		if(RFAILED(texture->LoadTexture(myDevice, filename)))
			ls << "Texture: " << filename << " Failed Load!\n";
	
	Core::Function_Exit();
	return OK;
}

RESULT D3DGraphObjFactory::CreateTextureFromMemory(ITexture*& texture, int width, char* memory, char* key)
{
	Core::Function_Enter("D3DGraphObjFactory::LoadTexture()");
	texture = new D3DTexture(key);
	if(!texture)
		return OUTOFMEMORY;
	Core* c = Core::Instance();
	if((!key) || (c->RegisterMediaObject(texture)))
		texture->CreateTextureFromMemory(myDevice, memory, width);
	
	Core::Function_Exit();
	return OK;
}

RESULT D3DGraphObjFactory::LoadTextureEx(char* filename, DWORD transColor, ITexture*& texture, char* key)
{
	// load a texture with a transparent color
	Core::Function_Enter("D3DGraphObjFactory::LoadTextureEx()");
	texture = new D3DTexture(key);
	if(!texture)
		return OUTOFMEMORY;
	Core* c = Core::Instance();
	if((!key) || (c->RegisterMediaObject(texture)))
		texture->LoadTextureEx(myDevice, filename, transColor);
	
	Core::Function_Exit();
	return OK;
}