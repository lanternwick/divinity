#include "StaticMesh.h"


StaticMesh::StaticMesh()
{
	IObject::IObject();
	myDim.x = 1.0f;
	myDim.y = 1.0f;
	myDim.z = 1.0f;

}

StaticMesh::~StaticMesh()
{
	myDevice.Release();
	myVB.Release();
	myIB.Release();

}

RESULT StaticMesh::Think()
{

	return OK;
}

RESULT StaticMesh::Render()
{
	MATRIX t;
	MATRIX t2;
	MATRIX t3;
	MATRIX t4;
	// turn lighting on
	myDevice->SetRenderState(RS_LIGHTING, TRUE);
	// turn culling off
	myDevice->SetRenderState(RS_CULLMODE, CULL_NONE);
	// set this mesh's material
	myDevice->SetMaterial(&myMat);
	// make the current texture NULL
	ITexture::MakeNull(myDevice);
	// get the current transformation matrix
	myDevice->GetTransform(T_WORLD, &t);
	// create a scaling matrix based on dimensions
	MatrixMatrixScaling(&t3, myDim.x, myDim.y, myDim.z);
	// make a scaling matrix to make the mesh the size of myDim
	MatrixMatrixMultiply(&t2, &scale, &t3);
	// multiply with current matrix
	MatrixMatrixMultiply(&t4, &t2, &t);
	// set the transform
	myDevice->SetTransform(T_WORLD, &t4);
	// if this mesh uses indices
	if(indexed) 
	{
		// set both VB and IB
		myVB->Set(myDevice);
		myIB->Set(myDevice);

		// and draw the mesh
		myDevice->DrawIndexedPrim(PT_TRILIST, 0, numV, 0, numF);
	}
	else
	{
		// set just the VB
		myVB->Set(myDevice);

		// and draw the mesh
		myDevice->DrawPrim(PT_TRILIST, 0, numV / 3);
	}
	
	// turn lighting off
	myDevice->SetRenderState(RS_LIGHTING, FALSE);
	return OK;
}

RESULT StaticMesh::LoadMesh(CSmartPtr<IGraphObjFactory> factory, char* file)
// Load a mesh from a file.   
// FileGuard Closes file at function end
{
	FILE* f = NULL;
	float* verts;
	WORD* indices;
	DWORD numVerts = 0;
	DWORD numIndices = 0;
	DWORD numFaces = 0;
	DWORD typeID = 0;
	
	f = fopen(file, "r+b"); // open file in binary

	FileGuard fg(f);
	if(fg == NULL)
		return FAILED_OPEN;

	fread(&typeID, 1, 4, fg); // get Mesh Type
	if(typeID != ID_STATICMESH) // if its not a static mesh
		return OBJECTINVALID; // return
	
	fread(&numVerts, 1, 4, fg); // get the number of vertices
	if(numVerts == 0) // if there arent any
		return OBJECTINVALID; // return

	numV = numVerts; 
	fread(&numIndices, 1, 4, fg); // get the number of indices
	indexed = FALSE;
	if(numIndices) // if there are indices
		indexed = TRUE; // this mesh in indexed
	fread(&numFaces, 1, 4, fg); // get num faces
	numF = numFaces;

	verts = new float[numVerts*8]; // create vertex data buffer
	if(verts == NULL)
		return OUTOFMEMORY;

	
	if(indexed)
	{
		indices = new WORD[numIndices]; // create new index data buffer
		if(indices == NULL)
		{
			delete [] verts;
			return OUTOFMEMORY;
		}
		numI = numIndices;
	}

	fread(verts, sizeof(float), numVerts*8, fg); // read in vertices
	if(indexed)
		fread(indices, sizeof(WORD), numIndices, fg); // read in indices

	ZeroMemory(&myMat, sizeof(MATERIAL)); // clear material
	fread(&myMat, sizeof(MATERIAL), 1, f); // and read it in

	// create a vertex buffer
	char buff[256];
	sprintf(buff, "%s_vb", file);
	RESULT r = factory->CreateVertexBuffer(numVerts, sizeof(TNVERTEX), USAGE_WRITEONLY, TNVERTFVF, myVB, buff);
	if(RFAILED(r))
	{
		delete [] verts;
		delete [] indices;
		return r;
	}
	if(indexed)
	{
		// create index buffer
		sprintf(buff, "%s_ib", file);
		r = factory->CreateIndexBuffer(numIndices, USAGE_WRITEONLY, myIB, buff);
		if(RFAILED(r))
		{
			delete [] verts;
			delete [] indices;
			return r;
		}
	}
	
	// fill the vertex buffer with data
	TNVERTEX* p_verts = NULL;
	myVB->Lock(0, 0, (BYTE**)&p_verts, 0);
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
	myVB->Unlock();
	// get max dimensions
	float maxX=verts[0], maxY=verts[1], maxZ=verts[2];
	for(i = 0; i < numVerts; ++i)
	{
		if(maxX < verts[i*8])
			maxX = verts[i*8];
		if(maxY < verts[i*8+1])
			maxY = verts[i*8+1];
		if(maxZ < verts[i*8+2])
			maxZ = verts[i*8+2];
	}
	// and create a scaling matrix so that the mesh is 1.0fx1.0fx1.0f
	MatrixMatrixScaling(&scale, 1.0f/maxX, 1.0f/maxY, 1.0f/maxZ);

	if(indexed)
	{
		// copy index data into the index buffer
		WORD* p_indices = NULL;
		myIB->Lock(0, 0, (BYTE**)&p_indices, 0);
		memcpy(p_indices, indices, numIndices*2);
		myIB->Unlock();
	}
	delete [] verts;
	delete [] indices;
	return OK;
}

RESULT StaticMesh::ExportMesh(char* file)
{
	// save the mesh to a file
	FILE* f = NULL;
	float* verts = NULL;
	WORD* indices = NULL;

	f = fopen(file, "w+b");
	if(!f)
		return FAILED_OPEN;
	FileGuard fg(f);
	DWORD data;
	// print mesh data
	data = ID_STATICMESH;
	fwrite(&data, 1, 4, fg);
	fwrite(&numV, 1, 4, fg);
	fwrite(&numI, 1, 4, fg);
	fwrite(&numF, 1, 4, fg);
	// and write out the other data
	myVB->Lock(0, 0, (BYTE**)&verts, 0);
	fwrite(&verts, sizeof(float)*8, numV, fg);
	myVB->Unlock();
	myIB->Lock(0, 0, (BYTE**)&indices, 0);
	fwrite(&indices, sizeof(WORD), numI, fg);
	myIB->Unlock();
	return OK;
}