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

unsigned long bin_pow(char ex)
{
	unsigned long ret=1;
	//if(ex)
	//	ex = ex;
	while(--ex>=0)
	{
		ret*=2;
	}
	//if(ret > 2)
	//	ret = ret;
	return ret;
}

IDevice* Terrain_Tile::m_Device;
IHeightMap* Terrain_Tile::m_heightMap = NULL;
ITextureMap* Terrain_Tile::m_textureMap = NULL;

Terrain_Tile::Terrain_Tile()
{
	
	
	m_ucScale = 0;
	m_verts = 0;
	m_indices = 0;
	m_vertices = NULL;
	m_vertices2 = NULL;
	m_indices_ = NULL;
	m_draw_shadows = true;


}


Terrain_Tile::~Terrain_Tile()
{

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
	for(int i = 0; i < m_tris.size(); ++i)
	{
		if(m_tris[i].ib)
			delete m_tris[i].ib;
	}
	m_tris.clear();
	//(*m_Texture).Release();
	//if(m_Texture) delete [] m_Texture;

}



void Terrain_Tile::SetDevice(IDevice* device)
{

	m_Device = device;
}

RESULT Terrain_Tile::Create(float x, float z, float scale, unsigned char subDivision)
{
	

	m_verts = (subDivision+1) * (subDivision+1);
	m_indices = subDivision*subDivision*6;
	try
	{
		m_Device->CreateVertexBuffer(m_verts, sizeof(LT2VERTEX), USAGE_WRITEONLY, LT2VERTFVF, m_vertices, NULL);
		m_Device->CreateVertexBuffer(m_verts, sizeof(LVERTEX), USAGE_WRITEONLY, LVERTFVF, m_vertices2, NULL);
		//for(int i = 0; i < m_textureMap->size(); ++i)
		m_Device->CreateIndexBuffer(m_indices, USAGE_WRITEONLY, 16, m_indices_, NULL);
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
	char tri[3];
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
			unsigned char mmcol = m_textureMap->GetTextureID(i+x, j+z);
			shadowVerts[c] = LVertex(x_t, m_heightMap->GetHeight(x_t, z_t), z_t, ARGB(tr, 0.0f, 0.0f, 0.0f));
			tileVerts[c++] = LT2Vertex(x_t, m_heightMap->GetHeight(x_t, z_t), z_t, mmcol%2 ? 0xff000000  : 0x00000000, 
				x_t/2, z_t/2, x_t/2, z_t/2);
			lx = x_t;
			lz = z_t;
		}
	}
	int w = sqrt(c);
	for(int ch = 0; ch < c; ++ch)
	{
		
		Color32 color;
		int alpha = 0;
		int div = 1;
		if(ch % w != 0) 
		{
			color.color = shadowVerts[ch-1].color;
			alpha += color.argb[3];
			++div;
		}
		if(ch % w != w-1)
		{
			color.color = shadowVerts[ch+1].color;
			alpha += color.argb[3];
			++div;
		}
		if(ch > w)
		{
			color.color = shadowVerts[ch-w].color;
			alpha += color.argb[3];
			++div;
		}
		if(ch < c-w)
		{
			color.color = shadowVerts[ch+w].color;
			alpha += color.argb[3];
			++div;
		}
		color.color = shadowVerts[ch].color;
		alpha += color.argb[3];
		alpha /= div;
		shadowVerts[ch].color = ARGB(float(alpha)/ float(256), 0, 0, 0);
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
	unsigned long d = 0;
	m_indices_->Lock(0, 0, (BYTE**)&indices, 0);
	tri_set_init* cur;
	std::vector<tri_set_init> tri_soup;
	WORD s = subDivision+1;
	for(i = 0; i < subDivision; ++i)
	{
		for(j = 0; j < subDivision; ++j)
		{
			//if((maxheight == _0) || (maxheight == _3))
			{
				tri[0] = m_textureMap->GetTextureID(i+x, j+z);
				tri[1] = m_textureMap->GetTextureID(i+x, j+z+1);
				tri[2] = m_textureMap->GetTextureID(i+x+1, j+z);

				if((tri[0] == tri[1]) || (tri[1] == tri[2]) || (tri[0] == tri[2]))
				{
					//triangle is valid;
					cur = NULL;
					unsigned long this_id = bin_pow(tri[0]) | bin_pow(tri[1]) | bin_pow(tri[2]);
					for(int a = 0; a < tri_soup.size(); ++a)
					{
						if(tri_soup[a].id == this_id)
						{
							cur = &tri_soup[a];
						}
					}
					if(cur == NULL)
					{
						tri_soup.resize(a+1);
						cur = &tri_soup[a];
						cur->tid1 = tri[0];
						cur->tid2 = (tri[0] == tri[1] ? tri[2] : tri[1]);
						cur->id = this_id;
					}
					short this_ind;
					this_ind = i*s+j;
					cur->ind.push_back(this_ind);
					this_ind = i*s+1+j;
					cur->ind.push_back(this_ind);
					this_ind = (i+1)*s+j;
					cur->ind.push_back(this_ind);
				}
				else
				{
					//this triangle is not valid  =(
					//Core::WriteLog("Invalid triangle in terrain geometry creation.  Triangles may have a maximum of two textures!\n", 0);
					throw 0;
				}

				tri[0] = m_textureMap->GetTextureID(i+x, j+z+1);
				tri[1] = m_textureMap->GetTextureID(i+1+x, j+z+1);
				tri[2] = m_textureMap->GetTextureID(i+x+1, j+z);

				if((tri[0] == tri[1]) || (tri[1] == tri[2]) || (tri[0] == tri[2]))
				{
					//triangle is valid;
					cur = NULL;
					unsigned long this_id = bin_pow(tri[0]) | bin_pow(tri[1]) | bin_pow(tri[2]);
					for(int a = 0; a < tri_soup.size(); ++a)
					{
						if(tri_soup[a].id == this_id)
						{
							cur = &tri_soup[a];
						}
					}
					if(cur == NULL)
					{
						tri_soup.resize(a+1);
						cur = &tri_soup[a];
						cur->tid1 = tri[0];
						cur->tid2 = (tri[0] == tri[1] ? tri[2] : tri[1]);
						cur->id = this_id;
					}
					short this_ind;
					this_ind = i*s+1+j;
					cur->ind.push_back(this_ind);
					this_ind = (i+1)*s+1+j;
					cur->ind.push_back(this_ind);
					this_ind = (i+1)*s+j;
					cur->ind.push_back(this_ind);
				}
				else
				{
					//this triangle is not valid  =(
					//Core::WriteLog("Invalid triangle in terrain geometry creation.  Triangles may have a maximum of two textures!\n", 0);
					throw 0;
				}
				
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
	//lsd << "Finalizing tile triangle sets..\n";
	m_tris.resize(tri_soup.size());
	for(int h = 0; h < m_tris.size(); ++h)
	{
		m_Device->CreateIndexBuffer(tri_soup[h].ind.size(), USAGE_WRITEONLY, 16, m_tris[h].ib, NULL);
		m_tris[h].ib->Lock(0, 0, (BYTE**)&(m_tris[h].buffer), 0);
		memcpy(m_tris[h].buffer, &tri_soup[h].ind[0], tri_soup[h].ind.size()*2);
		m_tris[h].ib->Unlock();
		m_tris[h].tid1 = tri_soup[h].tid1;
		m_tris[h].tid2 = tri_soup[h].tid2;
		m_tris[h].numI = tri_soup[h].ind.size();
	}
	m_scale = scale;
	m_x = x;
	m_z = z;
	return OK;
}

void Terrain_Tile::Dirty()
{
	m_dirty = true;
}

RESULT Terrain_Tile::Render()
{

		
	
	m_vertices->Set();
	ITexture* tex = NULL;
	/*m_Device->SetTextureState(0, TS_COLOROP, TOP_SELECTARG1);
	m_Device->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
	m_Device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	m_Device->SetTextureState(1, TS_COLOROP, TOP_BLENDDIFFUSEALPHA);
	m_Device->SetTextureState(0, TS_ALPHAOP, TOP_SELECTARG1);
	m_Device->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);*/
	for(int i = 0; i < m_tris.size(); ++i)
	{
		tex = m_textureMap->GetTexture(m_tris[i].tid1);
		if(tex)
			tex->Set(0);
		tex = m_textureMap->GetTexture(m_tris[i].tid2);
		if(tex)
			tex->Set(1);
		m_tris[i].ib->Set();
		m_Device->SetTextureState(0, TS_COLOROP, TOP_SELECTARG1);
		m_Device->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
		m_Device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
		m_Device->SetTextureState(1, TS_COLOROP, TOP_BLENDDIFFUSEALPHA);
		m_Device->SetTextureState(0, TS_ALPHAOP, TOP_SELECTARG1);
		m_Device->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);
		m_Device->SetTextureState(0, TS_MAGFILTER, TEXF_LINEAR);
		m_Device->SetTextureState(0, TS_MINFILTER, TEXF_LINEAR);
		m_Device->DrawIndexedPrim(PT_TRILIST, 0, m_verts, 0, m_tris[i].numI/3);
	}
	if(m_draw_shadows)
	{
		m_indices_->Set();
		m_vertices2->Set();
		ITexture::MakeNull(m_Device, 0);
		m_Device->DrawIndexedPrim(PT_TRILIST, 0, m_verts, 0, m_indices/3);
	}

	return OK;
}

RESULT Terrain_Tile::Think()
{

	return OK;
}

void Terrain_Tile::SetHeightMap(IHeightMap* hMap)
{

	m_heightMap = hMap;

}

void Terrain_Tile::SetTextureMap(ITextureMap* tMap)
{
	
	m_textureMap = tMap;

}

void Terrain_Tile::SetTileID(unsigned long newID)
{
	
	m_tileID = newID;
	
}

unsigned long Terrain_Tile::GetTileID()
{
	
	
	
	return m_tileID;
}

bool Terrain_Tile::IsVisible(CCamera* cam)
{
		
	return false;
}

void Terrain_Tile::ToggleShadows()
{
	m_draw_shadows = !m_draw_shadows;
}