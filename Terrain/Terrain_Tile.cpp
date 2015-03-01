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

CSmartPtr<IGraphObjFactory> Terrain_Tile::m_factory;
CSmartPtr<IDevice> Terrain_Tile::m_Device;
//CSmartPtr<ITexture>* Terrain_Tile::m_Texture = NULL;

Terrain_Tile::Terrain_Tile()
{
	m_simple = FALSE;
	
	m_ucScale = 0;
	m_verts = 0;
	m_indices = 0;

}

Terrain_Tile::Terrain_Tile(int numTextures)
{
	//m_Texture = new CSmartPtr<ITexture>[numTextures];
	Terrain_Tile();
	
}

Terrain_Tile::~Terrain_Tile()
{
	m_Device.Release();
	m_factory.Release();
	m_ib.Release();
	m_vb.Release();
	m_simpleIB.Release();
	//(*m_Texture).Release();
	//if(m_Texture) delete [] m_Texture;
}

void Terrain_Tile::SetFactory(CSmartPtr<IGraphObjFactory>& factory)
{
	m_factory = factory;
}

void Terrain_Tile::SetDevice(CSmartPtr<IDevice>& device)
{
	m_Device = device;
}

RESULT Terrain_Tile::Create(VECTOR* corner0, VECTOR* corners, float scale, unsigned char subDivision)
{
	corners[0] = *corner0;
	corners[1] = *corner0;
	corners[2] = *corner0;
	corners[3] = *corner0;
	
	corners[1].x+=scale;
	corners[1].y+=corners->x;
	corners[2].z+=scale;
	corners[2].y+=corners->y;
	corners[3].x+=scale;
	corners[3].z+=scale;
	corners[3].y+=corners->z;

	m_verts = (subDivision+1) * (subDivision+1);
	m_indices = m_verts*6;

	try
	{
		char* idBuff = NULL;
		idBuff = new char[64];
		sprintf(idBuff, "%f%fvb", corner0->x, corner0->z);
		RESULT r = m_factory->CreateVertexBuffer(m_verts, sizeof(LTVERTEX), USAGE_WRITEONLY, LTVERTFVF, m_vb, idBuff);
		if(RFAILED(r))
			throw r;
		sprintf(idBuff, "%f%fib", corner0->x, corner0->z);
		r = m_factory->CreateIndexBuffer(m_indices, USAGE_WRITEONLY, m_ib, idBuff);
		if(RFAILED(r))
			throw r;
		sprintf(idBuff, "%f%fsimpleIB", corner0->x, corner0->z);
		r = m_factory->CreateIndexBuffer(6, USAGE_WRITEONLY, m_simpleIB, idBuff);
		if(RFAILED(r))
			throw r;
	}
	catch(RESULT r)
	{
		if(!m_vb.Null())
		{
			m_vb->Release();
			m_vb.Release();
		}
		if(!m_ib.Null())
		{
			m_ib->Release();
			m_ib.Release();
		}
		if(!m_simpleIB.Null())
		{
			m_simpleIB->Release();
			m_simpleIB.Release();
		}
		return r;
	}
	LTVERTEX* tileVerts;
	WORD* indices;
	m_vb->Lock(0, 0, (BYTE**)&tileVerts, 0);
	int i = 0;
	int j = 0;
	int c = 0;
	float step = float(1.0/float(subDivision));
	float x, z;
	for(i = 0; i < subDivision + 1; ++i)
	{
		for(j = 0; j < subDivision + 1; ++j)
		{
			x = corner0->x+step*scale*float(i);
			z = corner0->z+step*scale*float(j);
			tileVerts[c++] = LTVertex(x, corners[0].y+m_fractalHeight(float(i)/float(subDivision), float(j)/float(subDivision)), z, 0xffffffff, float(i)/float(subDivision), float(j)/float(subDivision));
		}
	}
	m_vb->Unlock();
	
	float maxheight = corners[0].y;
	maxheight = corners[0].y > maxheight ? corners[0].y : maxheight;
	maxheight = corners[1].y > maxheight ? corners[1].y : maxheight;
	maxheight = corners[2].y > maxheight ? corners[2].y : maxheight;
	maxheight = corners[3].y > maxheight ? corners[3].y : maxheight;

	//WORD* indices;
	c = 0;
	m_ib->Lock(0, 0, (BYTE**)&indices, 0);
	WORD s = subDivision+1;
	for(i = 0; i < subDivision; ++i)
	{
		for(j = 0; j < subDivision; ++j)
		{
			if((maxheight == corners[0].y) || (maxheight == corners[3].y))
			{
				indices[c++] = i*s+j;
				indices[c++] = i*s+1+j;
				indices[c++] = (i+1)*s+j;
				
				indices[c++] = i*s+1+j;
				indices[c++] = (i+1)*s+j;
				indices[c++] = (i+1)*s+1+j;
			}
			else
			{
				indices[c++] = i*s+j;
				indices[c++] = i*s+1+j;
				indices[c++] = (i+1)*s+1+j;
				
				indices[c++] = i*s+j;
				indices[c++] = (i+1)*s+1+j;
				indices[c++] = (i+1)*s+j;
			}
		}
	}
	m_ib->Unlock();
	m_simpleIB->Lock(0, 0, (BYTE**)&indices, 0);
	
	c = 0;
	if((maxheight == corners[0].y) || (maxheight == corners[3].y))
	{
		indices[c++] = 0;
		indices[c++] = (subDivision);
		indices[c++] = (subDivision+1)*(subDivision);
		
		indices[c++] = (subDivision);
		indices[c++] = (subDivision+1)*(subDivision);
		indices[c++] = (subDivision+1)*(subDivision+1)-1;
	}
	else
	{
		indices[c++] = 0;
		indices[c++] = (subDivision);
		indices[c++] = (subDivision+1)*(subDivision);
		
		indices[c++] = (subDivision);
		indices[c++] = (subDivision+1)*(subDivision);
		indices[c++] = (subDivision+1)*(subDivision+1)-1;
	}
	m_simpleIB->Unlock();

	/*m_vb->Lock(0, 0, (BYTE**)&tileVerts, 0);
	tileVerts[0] = LTVertex(corners[0].x, 0.0f, corners[0].z, 0xffffffff, 0.0f, 0.0f);
	tileVerts[1] = LTVertex(corners[1].x, 0.0f, corners[1].z, 0xffffffff, 0.0f, 0.0f);
	tileVerts[2] = LTVertex(corners[2].x, 0.0f, corners[2].z, 0xffffffff, 0.0f, 0.0f);
	tileVerts[3] = LTVertex(corners[3].x, 0.0f, corners[3].z, 0xffffffff, 0.0f, 0.0f);
	m_vb->Unlock();
	m_ib->Lock(0, 0, (BYTE**)&indices, 0);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;
	m_ib->Unlock();*/
	return OK;
}

void Terrain_Tile::Dirty()
{
	m_dirty = true;
}

RESULT Terrain_Tile::Render()
{
	//if(!(*m_Texture).Null())
	//	(*m_Texture)->Set(m_Device);
	m_vb->Set(m_Device);
	if(m_simple)
	{
		m_simpleIB->Set(m_Device);
		m_Device->DrawIndexedPrim(PT_TRILIST, 0, m_verts, 0, 2);
	}
	else
	{
		m_ib->Set(m_Device);
		m_Device->DrawIndexedPrim(PT_TRILIST, 0, m_verts, 0, m_indices/3);
	}
	
	return OK;
}

RESULT Terrain_Tile::Think()
{

	return OK;
}

float Terrain_Tile::m_fractalHeight(float x, float z)
{
	if((x == 0.0f) || (x == 1.0f))
		return z*(corners[0].y-corners[2].y);
	if((z == 0.0f) || (z == 1.0f))
		return x*(corners[0].y-corners[2].y);
	return ((float(rand() % 10000))/10000.0f)/2.0f;
}

void Terrain_Tile::DrawSimple()
{
	m_simple = TRUE;
}

void Terrain_Tile::DrawComplex()
{
	m_simple = FALSE;
}