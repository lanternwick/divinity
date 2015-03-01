#include "TOW_SelectBox.h"

IDevice* TOW_SelectBox::m_device = NULL;
IGraphObjFactory* TOW_SelectBox::m_factory = NULL;
TOW_World* TOW_SelectBox::m_world = NULL;
CScene* TOW_SelectBox::m_scene = NULL;

TOW_SelectBox::TOW_SelectBox()
{
	
	m_vb = NULL;
	destroying = false;
	m_factory->CreateVertexBuffer(40, sizeof(LTVERTEX), USAGE_WRITEONLY | USAGE_DYNAMIC, LTVERTFVF, m_vb, NULL);
}

TOW_SelectBox::~TOW_SelectBox()
{
	Release();
}


RESULT TOW_SelectBox::Render()
{
	const float ybias = 0.3f;
	CCamera c;
	m_scene->GetCamera(&c);
	ray r;
	POINT p;
	UserInterface::Instance()->GetMousePos(&p);
	VECTOR vs(float(p.x), float(p.y), 0);
	VECTOR o;
	
	if(!destroying)
	{
		m_device->ComputeRay(&r, &vs, &c);
		//bool b = m_world->GetMap()->HeightMap()->RayCast(&o, &r.orig, &r.dir, 1.0f);
		PLANE pl;
		pl.a = pl.c = pl.d = 0.0f;
		pl.b = 1.0f;
		bool b = r.intersect_plane(&o, &pl);
		if(!b)	
		{
			return OK;
		}
	}
	else
	{
		o = m_end;
	}
	//float x, z;

	o.y = 0.0f;
	//o = m_start;
	int numCells = 0;
	const float half_wid = m_width / 2.0f;
	LTVERTEX* v;
	LTVERTEX* vi;
	
	m_vb->Lock(0, 0, (BYTE**)&v, LOCK_DISCARD);
	/*
	v[0] = LTVertex(m_start.x-m_width, 0.0f, m_start.z-m_width, m_color, 0.0f, 0.0f);
	v[1] = LTVertex(m_start.x-m_width, 0.0f, m_start.z+m_width, m_color, 0.0f, 1.0f);
	v[2] = LTVertex(m_start.x+m_width, 0.0f, m_start.z-m_width, m_color, 1.0f, 0.0f);

	v[3] = LTVertex(m_start.x-m_width, 0.0f, m_start.z+m_width, m_color, 0.0f, 1.0f);
	v[4] = LTVertex(m_start.x+m_width, 0.0f, m_start.z-m_width, m_color, 1.0f, 0.0f);
	v[5] = LTVertex(m_start.x+m_width, 0.0f, m_start.z+m_width, m_color, 1.0f, 1.0f);
	*/
	VECTOR ul, lr;
	ul.x = min(m_start.x, o.x);
	ul.z = min(m_start.z, o.z);
	lr.x = max(m_start.x, o.x);
	lr.z = max(m_start.z, o.z);
	v[0] = LTVertex(ul.x, 0.0f, ul.z, m_current_color, 0.5f, 0.5f);
	v[1] = LTVertex(ul.x-m_width, 0.0f, ul.z, m_current_color, 0.0f, 0.5f);
	v[2] = LTVertex(ul.x-m_width, 0.0f, ul.z-m_width, m_current_color, 0.0f, 0.0f);
	v[3] = LTVertex(ul.x, 0.0f, ul.z-m_width, m_current_color, 0.5f, 0.0f);
	v[4] = LTVertex(ul.x+m_width, 0.0f, ul.z-m_width, m_current_color, 1.0f, 0.0f);
	v[5] = LTVertex(ul.x+m_width, 0.0f, ul.z, m_current_color, 1.0f, 0.5f);
	v[6] = LTVertex(ul.x+m_width, 0.0f, ul.z+m_width, m_current_color, 1.0f, 1.0f);
	v[7] = LTVertex(ul.x, 0.0f, ul.z+m_width, m_current_color, 0.5f, 1.0f);
	v[8] = LTVertex(ul.x-m_width, 0.0f, ul.z+m_width, m_current_color, 0.0f, 1.0f);
	v[9] = LTVertex(ul.x-m_width, 0.0f, ul.z, m_current_color, 0.0f, 0.5f);

	v[10] = LTVertex(lr.x, 0.0f, ul.z, m_current_color, 0.5f, 0.5f);

	v[11] = LTVertex(lr.x-m_width, 0.0f, ul.z, m_current_color, 0.5f, 1.0f);
	v[12] = LTVertex(lr.x-m_width, 0.0f, ul.z-m_width, m_current_color, 0.0f, 0.5f);
	v[13] = LTVertex(lr.x, 0.0f, ul.z-m_width, m_current_color, 0.0f, 0.5f);
	v[14] = LTVertex(lr.x+m_width, 0.0f, ul.z-m_width, m_current_color, 0.0f, 0.0f);
	v[15] = LTVertex(lr.x+m_width, 0.0f, ul.z, m_current_color, 0.5f, 0.0f);
	v[16] = LTVertex(lr.x+m_width, 0.0f, ul.z+m_width, m_current_color, 1.0f, 0.0f);
	v[17] = LTVertex(lr.x, 0.0f, ul.z+m_width, m_current_color, 1.0f, 0.5f);
	v[18] = LTVertex(lr.x-m_width, 0.0f, ul.z+m_width, m_current_color, 1.0f, 1.0f);
	v[19] = LTVertex(lr.x-m_width, 0.0f, ul.z, m_current_color, 0.5f, 1.0f);

	v[20] = LTVertex(lr.x, 0.0f, lr.z, m_current_color, 0.5f, 0.5f);

	v[21] = LTVertex(lr.x-m_width, 0.0f, lr.z, m_current_color, 0.5f, 1.0f);
	v[22] = LTVertex(lr.x-m_width, 0.0f, lr.z-m_width, m_current_color, 1.0f, 0.5f);
	v[23] = LTVertex(lr.x, 0.0f, lr.z-m_width, m_current_color, 1.0f, 0.5f);
	v[24] = LTVertex(lr.x+m_width, 0.0f, lr.z-m_width, m_current_color, 1.0f, 0.0f);
	v[25] = LTVertex(lr.x+m_width, 0.0f, lr.z, m_current_color, 0.5f, 0.0f);
	v[26] = LTVertex(lr.x+m_width, 0.0f, lr.z+m_width, m_current_color, 0.0f, 0.0f);
	v[27] = LTVertex(lr.x, 0.0f, lr.z+m_width, m_current_color, 0.0f, 0.5f);
	v[28] = LTVertex(lr.x-m_width, 0.0f, lr.z+m_width, m_current_color, 0.0f, 1.0f);
	v[29] = LTVertex(lr.x-m_width, 0.0f, lr.z, m_current_color, 0.5f, 1.0f);

	v[30] = LTVertex(ul.x, 0.0f, lr.z, m_current_color, 0.5f, 0.5f);
	v[31] = LTVertex(ul.x-m_width, 0.0f, lr.z, m_current_color, 0.0f, 0.5f);
	v[32] = LTVertex(ul.x-m_width, 0.0f, lr.z-m_width, m_current_color, 0.0f, 1.0f);
	v[33] = LTVertex(ul.x, 0.0f, lr.z-m_width, m_current_color, 0.5f, 1.0f);
	v[34] = LTVertex(ul.x+m_width, 0.0f, lr.z-m_width, m_current_color, 1.0f, 1.0f);
	v[35] = LTVertex(ul.x+m_width, 0.0f, lr.z, m_current_color, 1.0f, 0.5f);
	v[36] = LTVertex(ul.x+m_width, 0.0f, lr.z+m_width, m_current_color, 1.0f, 0.0f);
	v[37] = LTVertex(ul.x, 0.0f, lr.z+m_width, m_current_color, 0.5f, 0.0f);
	v[38] = LTVertex(ul.x-m_width, 0.0f, lr.z+m_width, m_current_color, 0.0f, 0.0f);
	v[39] = LTVertex(ul.x-m_width, 0.0f, lr.z, m_current_color, 0.0f, 0.5f);
	
	
	for(vi=v;vi!=v+40;++vi)
		vi->y = m_world->GetMap()->HeightMap()->GetHeight(vi->x, vi->z) + ybias;
	m_vb->Unlock();
	m_vb->Set();
	ITexture* tex = (ITexture*)Core::GetMediaObject(moniker("selbox_corner"));
	tex->Set(0);
	m_device->DrawPrim(PT_TRIFAN, 0, 8);
	m_device->DrawPrim(PT_TRIFAN, 10, 8);
	m_device->DrawPrim(PT_TRIFAN, 20, 8);
	m_device->DrawPrim(PT_TRIFAN, 30, 8);
	
	m_end = o;
	
	return OK;
}

void TOW_SelectBox::SetStartPoint(VECTOR& v)
{
	m_start = v;
}

void TOW_SelectBox::Release()
{
	if(m_vb)
	{
		m_vb->Release();
		delete m_vb;
	}	
	m_vb = NULL;
}

void TOW_SelectBox::SetWidth(float width)
{
	m_width = width;
}

void TOW_SelectBox::SetColor(unsigned long color)
{
	m_color = color;
	m_current_color = m_color;
}

bool TOW_SelectBox::Destroy(float t)
{
	dest_t = t;
	destroying = true;
	m_current_color = m_color;
	return true;
}

bool TOW_SelectBox::DestroyTick()
{
	if(Core::GetTime() - dest_t > 0.5f)
		return false;
	m_current_color = ColorInterpolate(m_color, m_color & 0x00ffffff, (Core::GetTime() - dest_t) / 0.5f);
	return true;
}

void TOW_SelectBox::SetDevice(IDevice* device)
{
	m_device = device;
}

void TOW_SelectBox::SetFactory(IGraphObjFactory* factory)
{
	m_factory = factory;
}

void TOW_SelectBox::SetWorld(TOW_World* world)
{
	m_world = world;
}

void TOW_SelectBox::SetScene(CScene* scene)
{
	m_scene = scene;
}