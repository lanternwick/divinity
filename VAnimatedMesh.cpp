#include "VAnimatedMesh.h"
#include "..\Common\FileGuard.h"
#include "..\Common\types.h"

VAnimatedMesh::VAnimatedMesh()
{
	m_mutex.lock();
	
	
	IObject::IObject();
	myVB = NULL;
	myIB = NULL;
	
	myDim.x = 1.0f;
	myDim.y = 1.0f;
	myDim.z = 1.0f;
	m_mutex.unlock();
}

VAnimatedMesh::VAnimatedMesh(char* id)
{
	m_mutex.lock();
	m_id() = id;
	
	IObject::IObject();
	myVB = NULL;
	myIB = NULL;
	
	myDim.x = 1.0f;
	myDim.y = 1.0f;
	myDim.z = 1.0f;
	m_mutex.unlock();
}

VAnimatedMesh::~VAnimatedMesh()
{
	m_mutex.lock();
	for(int k = 0; k < m_anims.anims.size(); ++k)
	{
		for(int j = 0; j < m_anims.anims[k].frames.size(); ++j)
		{
			for(int i = 0; i < m_anims.anims[k].frames[j].m.subsets.size(); ++i)
			{
				m_anims.anims[k].frames[j].m.subsets[i].vb->Release();
				m_anims.anims[k].frames[j].m.subsets[i].ib->Release();
				delete m_anims.anims[k].frames[j].m.subsets[i].vb;
				delete m_anims.anims[k].frames[j].m.subsets[i].ib;
			}
		}
	}
	m_mutex.unlock();
}

RESULT VAnimatedMesh::Render()
{
	m_mutex.lock();
	animation* canim = NULL;
	for(std::vector<animation>::iterator i = m_anims.anims.begin(); i != m_anims.anims.end(); ++i)
	{
		if((*i).name() == m_curr_anim())
		{
			canim = i;
			break;
		}
	}
	if(!canim)
	{
		return OK;
	}
	
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
	float ti = Core::GetTime();
	float d = fmod(ti, canim->length);
	int f = d / canim->frames.size();
	frame* fr = &canim->frames[f];
	for(int s = 0; s < fr->m.numSubsets; ++s)
	{
		// create a scaling matrix based on dimensions
		MatrixMatrixScaling(&t3, myDim.x, myDim.y, myDim.z);
		// make a scaling matrix to make the mesh the size of myDim
		MatrixMatrixMultiply(&t2, &fr->m.subsets[s].scale, &t3);
		// multiply with current matrix
		MatrixMatrixMultiply(&t4, &t2, &t);
		// set the transform
		myDevice->SetTransform(T_WORLD, &t4);
		// set this mesh's material
		myDevice->SetMaterial(&fr->m.subsets[s].mat);
		fr->m.subsets[s].vb->Set();
		fr->m.subsets[s].ib->Set();
		myDevice->DrawIndexedPrim(PT_TRILIST, 0, fr->m.subsets[s].numVerts, 0, fr->m.subsets[s].numFaces);
	}

	myDevice->SetRenderState(RS_LIGHTING, FALSE);
	
	
	m_mutex.unlock();
	return OK;

}

RESULT VAnimatedMesh::Think()
{
	return OK;
}

RESULT VAnimatedMesh::LoadMesh(IGraphObjFactory* factory, char* file)
{
	m_mutex.lock();
	m_factory = factory;
	FILE* f = fopen(file, "r+b");
	stream_set(f, m_anims);
	fclose(f);

	m_mutex.unlock();
	return OK;
}

void VAnimatedMesh::DrawSubset(unsigned long subset)
{

}

void VAnimatedMesh::SetAnimation(moniker& animname)
{
	m_curr_anim = animname;
}

void VAnimatedMesh::stream_frame(FILE* f, VAnimatedMesh::frame& fr)
{
	m_mutex.lock();
	Core::Function_Enter("VAnimatedMesh::LoadMesh()");
	

	float* verts;
	WORD* indices;
	DWORD numVerts = 0;
	DWORD numIndices = 0;
	DWORD numFaces = 0;
	DWORD numSubsets = 0;
	DWORD typeID = 0;
	
	

	
	
	fread(&numSubsets, 1, 4, f);
	if(!numSubsets)
	{
		fr.m.numSubsets = 0;
		m_mutex.unlock();
		Core::Function_Exit();
		return;
	}
	fr.m.numSubsets = numSubsets;
	fr.m.subsets.resize(numSubsets);
	unsigned long i;
	float maxX=0.0f, maxY=0.0f, maxZ=0.0f;
	for(unsigned long j = 0; j < numSubsets; ++j)
	{
		fread(&numVerts, 1, 4, f); // get the number of vertices
		if(numVerts == 0) // if there arent any
		{
			
			continue;
		}

		fr.m.subsets[j].numVerts = numVerts; 
		fread(&numIndices, 1, 4, f); // get the number of indices
		
		/*if(numIndices) // if there are indices
			indexed = TRUE; // this mesh in indexed
		*/
		fread(&numFaces, 1, 4, f); // get num faces
		fr.m.subsets[j].numFaces = numFaces;

		verts = new float[numVerts*8]; // create vertex data buffer
		if(verts == NULL)
		{
			m_mutex.unlock();
			Core::Function_Exit();
			return;
		}

		
		indices = new WORD[numIndices]; // create new index data buffer
		if(indices == NULL)
		{
			delete [] verts;
			m_mutex.unlock();
			Core::Function_Exit();
			return;
		}
		fr.m.subsets[j].numIndices = numIndices;
	
		fread(verts, sizeof(float), numVerts*8, f); // read in vertices
		
		fread(indices, sizeof(WORD), numIndices, f); // read in indices

		ZeroMemory(&(fr.m.subsets[j].mat), sizeof(MATERIAL)); // clear material
		fread(&(fr.m.subsets[j].mat), sizeof(MATERIAL), 1, f); // and read it in

		// create a vertex buffer
		
		RESULT r = m_factory->CreateVertexBuffer(numVerts, sizeof(TNVERTEX), USAGE_WRITEONLY, TNVERTFVF, (fr.m.subsets[j].vb), NULL);
		if(RFAILED(r))
		{
			delete [] verts;
			delete [] indices;
			m_mutex.unlock();
			Core::Function_Exit();
			return;
		}
		
		r = m_factory->CreateIndexBuffer(numIndices, USAGE_WRITEONLY, fr.m.subsets[j].ib, NULL);
		if(RFAILED(r))
		{
			delete [] verts;
			delete [] indices;
			m_mutex.unlock();
			Core::Function_Exit();
			return;
		}
		
		
		// fill the vertex buffer with data
		TNVERTEX* p_verts = NULL;
		fr.m.subsets[j].vb->Lock(0, 0, (BYTE**)&p_verts, 0);
		//memcpy(p_verts, verts, m_mesh.subsets[j].numVerts*8);
		/*for(i = 0; i < numVerts; ++i)
		{
			p_verts[i] = TNVertex(verts[i*8], 
								  verts[i*8+1], 
								  verts[i*8+2], 
								  verts[i*8+3], 
								  verts[i*8+4], 
								  verts[i*8+5], 
								  verts[i*8+6], 
								  verts[i*8+7]);

		}*/
		memcpy(p_verts, verts, numVerts*8*4);
		fr.m.subsets[j].vb->Unlock();
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
		

		// copy index data into the index buffer
		WORD* p_indices = NULL;
		fr.m.subsets[j].ib->Lock(0, 0, (BYTE**)&p_indices, 0);
		memcpy(p_indices, indices, fr.m.subsets[j].numIndices*2);
		fr.m.subsets[j].ib->Unlock();
		
		delete [] verts;
		delete [] indices;
	}
	for(i = 0; i < fr.m.numSubsets; ++i)
	{
		// and create a scaling matrix so that the mesh is 1.0fx1.0fx1.0f
		MatrixMatrixScaling(&(fr.m.subsets[i].scale), 1.0f/maxX, 1.0f/maxY, 1.0f/maxZ);
	}
	Core::Function_Exit();
	m_mutex.unlock();
	return;
}

void VAnimatedMesh::stream_anim(FILE* f, VAnimatedMesh::animation& anim)
{
	char name[32];
	unsigned long num_frames;
	memset(name, 0, 32);
	fread(name, 1, 32, f);
	char* n = name+30;
	for(;*n==' ';n--);
	*(n+1) = 0;
	anim.name() = name;
	fread(&num_frames, 1, sizeof(num_frames), f);
	anim.frames.resize(num_frames);
	anim.length = 3.0f;
	for(int i = 0; i < num_frames; ++i)
	{
		stream_frame(f, anim.frames[i]);
	}
}

void VAnimatedMesh::stream_set(FILE* f, VAnimatedMesh::animation_set& set)
{
	unsigned long id;
	unsigned long num_anims;
	fread(&id, 1, sizeof(id), f);
	if(id != ID_VANIMATEDMESH)
	{
		return;
	}
	fread(&num_anims, 1, sizeof(num_anims), f);
	set.anims.resize(num_anims);
	for(int i = 0; i < num_anims; ++i)
	{
		stream_anim(f, set.anims[i]);
	}
}

RESULT VAnimatedMesh::ExportMesh(char* file)
{
	return NOTIMPL;
}