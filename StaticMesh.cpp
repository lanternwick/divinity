#include "StaticMesh.h"


StaticMesh::StaticMesh()
{
	Core::Function_Enter("StaticMesh::Constructor()");
	m_mutex.lock();
	IObject::IObject();
	myVB = NULL;
	myIB = NULL;
	
	myDim.x = 1.0f;
	myDim.y = 1.0f;
	myDim.z = 1.0f;
	m_mutex.unlock();
	Core::Function_Exit();

}

StaticMesh::StaticMesh(char* id)
{
	Core::Function_Enter("StaticMesh::Constructor()");
	m_mutex.lock();
	IObject::IObject();
	
	myVB = NULL;
	myIB = NULL;
	
	myDim.x = 1.0f;
	myDim.y = 1.0f;
	myDim.z = 1.0f;
	m_id() = id;
	m_mutex.unlock();
	Core::Function_Exit();
}

StaticMesh::~StaticMesh()
{
	Core::Function_Enter("StaticMesh::Destructor()");
	m_mutex.lock();
	IMesh::~IMesh();
	for(std::vector<mesh_subset>::iterator i = m_mesh.subsets.begin(); i != m_mesh.subsets.end(); i++)
	{
		if((*i).vb)
		{
			(*i).vb->Release();
			delete (*i).vb;
		}
		if((*i).ib)
		{
			(*i).ib->Release();
			delete (*i).ib;
		}
	}
	m_mesh.subsets.resize(0);
	m_mutex.unlock();
	Core::Function_Exit();

}

RESULT StaticMesh::Think()
{
	Core::Function_Enter("StaticMesh::Think()");
	m_mutex.lock();
	m_mutex.unlock();
	Core::Function_Exit();
	return OK;
}

RESULT StaticMesh::Render()
{
	Core::Function_Enter("StaticMesh::Render()");
	m_mutex.lock();
	MATRIX t;
	MATRIX t2;
	MATRIX t3;
	MATRIX t4;
	// turn lighting on
	myDevice->SetRenderState(RS_LIGHTING, TRUE);
	// turn culling off
	myDevice->SetRenderState(RS_CULLMODE, CULL_NONE);
	myDevice->SetTextureState(0, TS_COLOROP, TOP_MODULATE);
	myDevice->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
	myDevice->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	//myDevice->SetRenderState(RS_WRAP0, WRAPCOORD_3);
	ITexture::MakeNull(myDevice, 1);
	
	// make the current texture NULL
	//ITexture::MakeNull(myDevice, 0);
	// get the current transformation matrix
	myDevice->GetTransform(T_WORLD, &t);
	for(int i = 0; i < m_mesh.subsets.size(); ++i)
	{

		// create a scaling matrix based on dimensions
		MatrixMatrixScaling(&t3, myDim.x, myDim.y, myDim.z);
		// make a scaling matrix to make the mesh the size of myDim
		MatrixMatrixMultiply(&t2, &m_mesh.subsets[i].scale, &t3);
		// multiply with current matrix
		MatrixMatrixMultiply(&t4, &t2, &t);
		// set the transform
		myDevice->SetTransform(T_WORLD, &t4);
		// set this mesh's material
		myDevice->SetMaterial(&m_mesh.subsets[i].mat);
		// if this mesh uses indices
		if(indexed) 
		{
			// set both VB and IB
			m_mesh.subsets[i].vb->Set();
			m_mesh.subsets[i].ib->Set();

			// and draw the mesh
			myDevice->DrawIndexedPrim(PT_TRILIST, 0, m_mesh.subsets[i].numVerts, 0, m_mesh.subsets[i].numFaces);
		}
		else
		{
			// set just the VB
			myVB->Set();

			// and draw the mesh
			//myDevice->DrawPrim(PT_TRILIST, 0, numV / 3);
		}
	}
	
	// turn lighting off
	myDevice->SetRenderState(RS_LIGHTING, FALSE);
	//myDevice->SetRenderState(RS_WRAP0, 0);
	m_mutex.unlock();
	Core::Function_Exit();
	return OK;
}

RESULT StaticMesh::LoadMesh(IGraphObjFactory* factory, char* file)
// Load a mesh from a file.   
// FileGuard Closes file at function end
{
	Core::Function_Enter("StaticMesh::LoadMesh()");
	m_mutex.lock();
	FILE* f = NULL;
	float* verts;
	WORD* indices;
	DWORD numVerts = 0;
	DWORD numIndices = 0;
	DWORD numFaces = 0;
	DWORD numSubsets = 0;
	DWORD typeID = 0;
	
	f = fopen(file, "r+b"); // open file in binary

	FileGuard fg(f);
	if(fg == NULL)
	{
		m_mutex.unlock();
		Core::Function_Exit();
		return FAILED_OPEN;
	}
	
	fread(&typeID, 1, 4, fg); // get Mesh Type
	if(typeID != ID_STATICMESH) // if its not a static mesh
	{
		m_mutex.unlock();
		Core::Function_Exit();
		return OBJECTINVALID; // return
	}
	fread(&numSubsets, 1, 4, fg);
	if(!numSubsets)
	{
		m_mesh.numSubsets = 0;
		m_mutex.unlock();
		Core::Function_Exit();
		return OBJECTINVALID;
	}
	m_mesh.numSubsets = numSubsets;
	m_mesh.subsets.resize(numSubsets);
	unsigned long i;
	float maxX=0.0f, maxY=0.0f, maxZ=0.0f;
	for(unsigned long j = 0; j < numSubsets; ++j)
	{
		fread(&numVerts, 1, 4, fg); // get the number of vertices
		if(numVerts == 0) // if there arent any
		{
			
			continue;
		}

		m_mesh.subsets[j].numVerts = numVerts; 
		fread(&numIndices, 1, 4, fg); // get the number of indices
		indexed = FALSE;
		if(numIndices) // if there are indices
			indexed = TRUE; // this mesh in indexed
		fread(&numFaces, 1, 4, fg); // get num faces
		m_mesh.subsets[j].numFaces = numFaces;

		verts = new float[numVerts*8]; // create vertex data buffer
		if(verts == NULL)
		{
			m_mutex.unlock();
			Core::Function_Exit();
			return OUTOFMEMORY;
		}

		
		if(indexed)
		{
			indices = new WORD[numIndices]; // create new index data buffer
			if(indices == NULL)
			{
				delete [] verts;
				m_mutex.unlock();
				Core::Function_Exit();
				return OUTOFMEMORY;
			}
			m_mesh.subsets[j].numIndices = numIndices;
		}

		fread(verts, sizeof(float), numVerts*8, fg); // read in vertices
		if(indexed)
			fread(indices, sizeof(WORD), numIndices, fg); // read in indices

		ZeroMemory(&m_mesh.subsets[j].mat, sizeof(MATERIAL)); // clear material
		fread(&m_mesh.subsets[j].mat, sizeof(MATERIAL), 1, fg); // and read it in

		// create a vertex buffer
		
		RESULT r = factory->CreateVertexBuffer(numVerts, sizeof(TNVERTEX), USAGE_WRITEONLY, TNVERTFVF, m_mesh.subsets[j].vb, NULL);
		if(RFAILED(r))
		{
			delete [] verts;
			delete [] indices;
			m_mutex.unlock();
			Core::Function_Exit();
			return r;
		}
		if(indexed)
		{
			// create index buffer
			
			r = factory->CreateIndexBuffer(numIndices, USAGE_WRITEONLY, m_mesh.subsets[j].ib, NULL);
			if(RFAILED(r))
			{
				delete [] verts;
				delete [] indices;
				m_mutex.unlock();
				Core::Function_Exit();
				return r;
			}
		}
		
		// fill the vertex buffer with data
		TNVERTEX* p_verts = NULL;
		m_mesh.subsets[j].vb->Lock(0, 0, (BYTE**)&p_verts, 0);
		//memcpy(p_verts, verts, m_mesh.subsets[j].numVerts*8);
		for(i = 0; i < numVerts; ++i)
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
		m_mesh.subsets[j].vb->Unlock();
		// get max dimensions
		//float maxX=verts[0], maxY=verts[1], maxZ=verts[2];
		for(i = 0; i < numVerts; ++i)
		{
			if(maxX < verts[i*8])
				maxX = verts[i*8];
			if(maxY < verts[i*8+1])
				maxY = verts[i*8+1];
			if(maxZ < verts[i*8+2])
				maxZ = verts[i*8+2];
		}
		

		if(indexed)
		{
			// copy index data into the index buffer
			WORD* p_indices = NULL;
			m_mesh.subsets[j].ib->Lock(0, 0, (BYTE**)&p_indices, 0);
			memcpy(p_indices, indices, m_mesh.subsets[j].numIndices*2);
			m_mesh.subsets[j].ib->Unlock();
		}
		delete [] verts;
		delete [] indices;
	}
	for(i = 0; i < m_mesh.numSubsets; ++i)
	{
		// and create a scaling matrix so that the mesh is 1.0fx1.0fx1.0f
		MatrixMatrixScaling(&m_mesh.subsets[i].scale, 1.0f/maxX, 1.0f/maxY, 1.0f/maxZ);
	}
	Core::Function_Exit();
	m_mutex.unlock();
	return OK;
}

RESULT StaticMesh::ExportMesh(char* file)
{
	// save the mesh to a file
	Core::Function_Enter("StaticMesh::ExportMesh()");
	/*FILE* f = NULL;
	float* verts = NULL;
	WORD* indices = NULL;

	f = fopen(file, "w+b");
	if(!f)
	{
		Core::Function_Exit();
		return FAILED_OPEN;
	}
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
	myIB->Unlock();*/
	Core::Function_Exit();
	return OK;
}

void StaticMesh::DrawSubset(unsigned long subset)
{
	m_mutex.lock();
	m_mutex.unlock();
}