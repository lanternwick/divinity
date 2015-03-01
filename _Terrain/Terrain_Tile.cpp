#include "Terrain_Tile.h"

// tile
// 0+-------------+1
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
// 2+-------------+3

IGraphObjFactory* Terrain_Tile::m_factory;
IDevice* Terrain_Tile::m_Device;
IHeightMap* Terrain_Tile::m_heightMap = NULL;

Terrain_Tile::Terrain_Tile()
{
	Core::Function_Enter("Terrain_Tile::Constructor()");
	
	m_ucScale = 0;
	m_verts = 0;
	m_indices = 0;
	m_vertices = NULL;
	m_indices_ = NULL;
	Core::Function_Exit();

}


Terrain_Tile::~Terrain_Tile()
{
	Core::Function_Enter("Terrain_Tile::Destructor()");
	if(m_indices_)
	{
	
		delete m_indices_;
	}
	if(m_vertices)
	{
	
		delete m_vertices;
	}
	if(m_vertices2)
	{
	
		delete m_vertices2;
	}
	//(*m_Texture).Release();
	//if(m_Texture) delete [] m_Texture;
	Core::Function_Exit();
}

void Terrain_Tile::SetFactory(IGraphObjFactory* factory)
{
	Core::Function_Enter("Terrain_Tile::SetFactory()");
	m_factory = factory;
	Core::Function_Exit();
}

void Terrain_Tile::SetDevice(IDevice* device)
{
	Core::Function_Enter("Terrain_Tile::SetDevice()");
	m_Device = device;
	Core::Function_Exit();
}

RESULT Terrain_Tile::Create(float x, float z, float scale, unsigned char subDivision)
{
	Core::Function_Enter("Terrain_Tile::Create()");
	
	m_verts = (subDivision+1) * (subDivision+1);
	m_indices = subDivision*subDivision*6;

	try
	{
		m_factory->CreateVertexBuffer(m_verts, sizeof(LT2VERTEX), USAGE_WRITEONLY, LT2VERTFVF, m_vertices, NULL);
		m_factory->CreateVertexBuffer(m_verts, sizeof(LVERTEX), USAGE_WRITEONLY, LVERTFVF, m_vertices2, NULL);
		m_factory->CreateIndexBuffer(m_indices, USAGE_WRITEONLY, m_indices_, NULL);
	}
	catch(...)
	{
		if(m_vertices)
		{
			delete m_vertices;
			m_vertices = NULL;
		}
		if(m_vertices2)
		{
			delete m_vertices2;
			m_vertices2 = NULL;
			
		}
		if(m_indices_)
		{
			delete m_indices_;
			m_indices_ = NULL;
		}
		Core::Function_Exit();
		return OUTOFMEMORY;
	}
	LT2VERTEX* tileVerts;
	LVERTEX* shadowVerts;
	WORD* indices;
	
	int i = 0;
	int j = 0;
	int c = 0;
	float step = float(1.0/float(subDivision));
	float x_t, z_t, lx = 0.0f, lz = 0.0f;
	float tu, tv;
	m_vertices->Lock(0, 0, (BYTE**)&tileVerts, 0);
	m_vertices2->Lock(0, 0, (BYTE**)&shadowVerts, 0);
	
	unsigned long vertcol;
	float tr;
	VECTOR fVertex;
	VECTOR fSunDir(0.1f, -0.001f, 0.1f);
	PLANE n;
	for(i = 0; i < subDivision+1; ++i)
	{
		for(j = 0; j < subDivision+1; ++j)
		{
			x_t = x+step*scale*float(i);
			z_t = z+step*scale*float(j);
			vertcol = 1 ? 0xff000000 : 0x00000000;
			fVertex.x = x_t;
			fVertex.y = m_heightMap->GetHeight(x_t, z_t);
			fVertex.z = z_t;;
			//fVertex + fSunDir * 2000.0f;
			//r.direction = fSunDir;
			if(!m_heightMap->Occluded(&fVertex, &(VECTOR(0.0f, 3.0f, 0.0f)), &(fSunDir)))
			{
				tr = 0.65f;
				PlaneFromPoints(&n, &VECTOR(x_t-1.0f, m_heightMap->GetHeight(x_t-1.0f, z_t-1.0f), z_t-1.0f),
								&VECTOR(x_t+1.0f, m_heightMap->GetHeight(x_t+1.0f, z_t-1.0f), z_t-1.0f),
								&VECTOR(x_t-1.0f, m_heightMap->GetHeight(x_t-1.0f, z_t+1.0f), z_t+1.0f));
				fVertex.x = n.a;
				fVertex.y = n.b;
				fVertex.z = n.c;
				VectorNormalize(&fVertex, &fVertex);
				VectorNormalize(&fSunDir, &fSunDir);
				tr -= 0.5f*(0.75f+DotProduct(&fVertex, &fSunDir));
				if (tr<0.0f) tr = 0.0f;

			}
			else
			{	
				tr = 0.1f;
				PlaneFromPoints(&n, &VECTOR(x_t-1.0f, m_heightMap->GetHeight(x_t-1.0f, z_t-1.0f), z_t-1.0f),
								&VECTOR(x_t+1.0f, m_heightMap->GetHeight(x_t+1.0f, z_t-1.0f), z_t-1.0f),
								&VECTOR(x_t-1.0f, m_heightMap->GetHeight(x_t-1.0f, z_t+1.0f), z_t+1.0f));
				fVertex.x = n.a;
				fVertex.y = n.b;
				fVertex.z = n.c;
				VectorNormalize(&fVertex, &fVertex);
				VectorNormalize(&fSunDir, &fSunDir);
				tr += 0.5f*(0.75f+DotProduct(&fVertex, &fSunDir));
				if (tr>0.8f) tr = 0.8f;
				//tr = 0.3f;
			}
			//tr = (m_heightMap->GetHeight(x_t, z_t) - m_heightMap->GetHeight(x+step*scale*float(i-1), z+step*scale*float(j+1)));
			/*if(tr < 0.0f)
			{
				tr = 0.8f - -tr;
			}
			else
			{
				tr = 0.3f;
			}*/
			//vertcol = ARGB(a, 1.0f, 1.0f, 1.0f);
				
			tu = float(i)/float(subDivision);
			tv = float(j)/float(subDivision);

			shadowVerts[c] = LVertex(x_t, m_heightMap->GetHeight(x_t, z_t), z_t, ARGB(tr, 0.0f, 0.0f, 0.0f));
			tileVerts[c++] = LT2Vertex(x_t, m_heightMap->GetHeight(x_t, z_t), z_t, !(rand()%8) ? 0xff000000 : 0x00000000, 
				x_t/2, z_t/2, x_t/2, z_t/2);
			lx = x_t;
			lz = z_t;
		}
	}
	m_vertices->Unlock();
	m_vertices2->Unlock();
	
	x_t = x;
	z_t = z;
	float maxheight = m_heightMap->GetHeight(x_t, z_t);
	float _0 = maxheight, _3;
	float t = m_heightMap->GetHeight(x_t, z_t);
	maxheight = t > maxheight ? t : maxheight;
	t = m_heightMap->GetHeight(x_t+scale, z_t);
	maxheight = t > maxheight ? t : maxheight;
	t = m_heightMap->GetHeight(x_t+scale, z_t+scale);
	_3 = t;
	maxheight = t > maxheight ? t : maxheight;
	
	//WORD* indices;
	c = 0;
	m_indices_->Lock(0, 0, (BYTE**)&indices, 0);
	WORD s = subDivision+1;
	for(i = 0; i < subDivision; ++i)
	{
		for(j = 0; j < subDivision; ++j)
		{
			//if((maxheight == _0) || (maxheight == _3))
			{
				indices[c++] = i*s+j;
				indices[c++] = i*s+1+j;
				indices[c++] = (i+1)*s+j;
				
				indices[c++] = i*s+1+j;
				indices[c++] = (i+1)*s+1+j;
				indices[c++] = (i+1)*s+j;
			}
			/*else
			{
				indices[c++] = i*s+j;
				indices[c++] = i*s+1+j;
				indices[c++] = (i+1)*s+1+j;
				
				indices[c++] = i*s+j;
				indices[c++] = (i+1)*s+j;
				indices[c++] = (i+1)*s+1+j;
			}*/
		}
	}
	
	m_indices_->Unlock();
	m_scale = scale;
	m_x = x;
	m_z = z;
	Core::Function_Exit();
	return OK;
}

void Terrain_Tile::Dirty()
{
	Core::Function_Enter("Terrain_Tile::Dirty()");
	m_dirty = true;
	Core::Function_Exit();
}

RESULT Terrain_Tile::Render()
{
	//Core::Function_Enter("Terrain_Tile::Render()");
		
	m_indices_->Set();
	m_vertices->Set();
	m_Device->DrawIndexedPrim(PT_TRILIST, 0, m_verts, 0, m_indices/3);
	m_vertices2->Set();
	ITexture::MakeNull(m_Device, 0);
	m_Device->DrawIndexedPrim(PT_TRILIST, 0, m_verts, 0, m_indices/3);

	//Core::Function_Exit();
	return OK;
}

RESULT Terrain_Tile::Think()
{
	//Core::Function_Enter("Terrain_Tile::Think()");
	//Core::Function_Exit();
	return OK;
}

void Terrain_Tile::SetHeightMap(IHeightMap* hMap)
{
	Core::Function_Enter("Terrain_Tile::SetHeightMap()");
	m_heightMap = hMap;
	Core::Function_Exit();
}

void Terrain_Tile::SetTileID(unsigned long newID)
{
	Core::Function_Enter("Terrain_Tile::SetTileID()");
	m_tileID = newID;
	Core::Function_Exit();
}

unsigned long Terrain_Tile::GetTileID()
{
	Core::Function_Enter("Terrain_Tile::GetTileID()");
	unsigned long l = m_tileID;
	Core::Function_Exit();
	return l;
}

bool Terrain_Tile::IsVisible(CCamera* cam)
{
	
	
	return false;
}