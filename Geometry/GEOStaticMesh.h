#ifndef _GEOSTATICMESH_H
#define _GEOSTATICMESH_H

#include "IMesh.h"
#include <types.h>

class GEOStaticMesh : public IMesh
{
public:
	GEOStaticMesh();
	~GEOStaticMesh();

	RESULT Update();
	RESULT Render(DWORD& numPrims);

	RESULT LoadMesh(char* file);
	RESULT ExportMesh(char* file);
private:
	BOOL indexed;
	DWORD numI;
	DWORD numV;
};

GEOStaticMesh::GEOStaticMesh()
{
	type = OBJ_MESH;
}

GEOStaticMesh::~GEOStaticMesh()
{


}

RESULT GEOStaticMesh::Update()
{

	return OK;
}

RESULT GEOStaticMesh::Render(DWORD& numPrims)
{
	SetRenderState(RS_LIGHTING, TRUE);
	SetRenderState(RS_AMBIENT, 0xff999999);
	SetMaterial(myMat);
	SetNullObject(OBJ_TEXTURE);
	if(indexed)
	{
		SelectObject(VB);
		SelectObject(IB);

		DrawIndexedTriList(0, numV, 0, numI / 3);
	}
	else
	{
		SelectObject(VB);

		DrawTriList(0, numV / 3);
	}
	
	SetRenderState(RS_LIGHTING, FALSE);
	return OK;
}

RESULT GEOStaticMesh::LoadMesh(char* file)
// Load a mesh from a file.  MemGuards clean up memory at fucntion end.  
// FileGuard Closes file at function end
{
	FILE* f = NULL;
	float* verts;
	WORD* indices;
	DWORD numVerts = 0;
	DWORD numIndices = 0;
	DWORD typeID = 0;
	
	f = fopen(file, "r+b");

	FileGuard fg(f);
	if(fg == NULL)
		return FAILED_OPEN;

	fread(&typeID, 1, 4, fg);
	if(typeID != ID_STATICMESH)
		return OBJECTINVALID;
	
	fread(&numVerts, 1, 4, fg);
	if(numVerts == 0)
		return OBJECTINVALID;

	numV = numVerts;
	fread(&numIndices, 1, 4, fg);
	indexed = FALSE;
	if(numIndices)
		indexed = TRUE;
	//MemGuard vGuard(new float[numVerts*8]);
	verts = new float[numVerts*8];
	if(verts == NULL)
		return OUTOFMEMORY;

	
	if(indexed)
	{
		indices = new WORD[numIndices];
		if(indices == NULL)
		{
			delete [] verts;
			return OUTOFMEMORY;
		}
		numI = numIndices;
	}

	fread(verts, sizeof(float), numVerts*8, fg);
	fread(indices, sizeof(WORD), numIndices, fg);
	ZeroMemory(&myMat, sizeof(MATERIAL));
	fread(&myMat, sizeof(MATERIAL), 1, f);

	RESULT r = CreateVertexBuffer(&VB, numVerts * sizeof(TNVERTEX), sizeof(TNVERTEX), NULL, TNVERTFVF);
	if(RFAILED(r))
	{
		delete [] verts;
		delete [] indices;
		return r;
	}
	if(indexed)
	{
		r = CreateIndexBuffer(&IB, numIndices * sizeof(WORD), NULL);
		if(RFAILED(r))
		{
			delete [] verts;
			delete [] indices;
			return r;
		}
	}
	SelectObject(VB);
	TNVERTEX* p_verts = NULL;
	LockVertexBuffer(0, 0, (BYTE**)&p_verts, 0);
	for(DWORD i = 0; i < numVerts; ++i)
	{
		p_verts[i] = TNVertex(verts[i*8], 
							  verts[i*8+1], 
							  verts[i*8+2], 
							  verts[i*8+3], 
							  verts[i*8+4], 
							  verts[i*8+5], 
							  verts[i*8+6], 
							  verts[i*8+7]);

	}
	UnlockVertexBuffer();
	if(indexed)
	{
		SelectObject(IB);
		WORD* p_indices = NULL;
		LockIndexBuffer(0, 0, (BYTE**)&p_indices, 0);
		memcpy(p_indices, indices, numIndices*2);
		UnlockIndexBuffer();
	}
	delete [] verts;
	delete [] indices;
	return OK;
}

RESULT GEOStaticMesh::ExportMesh(char* file)
{
	FILE* f = NULL;
	float* verts = NULL;
	WORD* indices = NULL;

	f = fopen(file, "w+b");
	if(!f)
		return FAILED_OPEN;
	FileGuard fg(f);
	DWORD data;
	data = ID_STATICMESH;
	fwrite(&data, 1, 4, fg);
	fwrite(&numV, 1, 4, fg);
	fwrite(&numI, 1, 4, fg);
	SelectObject(VB);
	LockVertexBuffer(0, 0, (BYTE**)&verts, 0);
	fwrite(&verts, sizeof(float)*8, numV, fg);
	UnlockVertexBuffer();
	SelectObject(IB);
	LockIndexBuffer(0, 0, (BYTE**)&indices, 0);
	fwrite(&indices, sizeof(WORD), numI, fg);
	UnlockVertexBuffer();
	return OK;
}

#endif