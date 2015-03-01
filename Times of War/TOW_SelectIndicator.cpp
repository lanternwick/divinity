#include "TOW_SelectIndicator.h"

IHeightMap* TOW_SelectIndicator::m_hgtmap = NULL;

TOW_SelectIndicator::TOW_SelectIndicator()
{
	m_vb = NULL;
	m_device = NULL;
	m_factory = NULL;
	m_go = NULL;
	r = 0.0f;
}

TOW_SelectIndicator::~TOW_SelectIndicator()
{
	Release();
}

RESULT TOW_SelectIndicator::Create(IDevice* device, IGraphObjFactory* factory)
{
	m_device = device;
	m_factory = factory;
	
	factory->CreateVertexBuffer(10, sizeof(LTVERTEX), USAGE_DYNAMIC | USAGE_WRITEONLY, LTVERTFVF, m_vb, NULL);
	return OK;
}

RESULT TOW_SelectIndicator::Render()
{
	const float ybias=0.3f;
	LTVERTEX* v;
	if(m_go)
		p = m_go->pos;
	if(m_go)
		r = m_go->property(std::string("bound_sphere_radius")).as_if_float();
	if(r == 0.0f)
		return OK;
	m_vb->Lock(0, 0, (BYTE**)&v, LOCK_DISCARD);
	v[0] = LTVertex(p.x, 0.0f, p.z, m_current_color, 0.5f, 0.5f);
	v[1] = LTVertex(p.x-r, 0.0f, p.z, m_current_color, 0.0f, 0.5f);
	v[2] = LTVertex(p.x-r, 0.0f, p.z-r, m_current_color, 0.0f, 0.0f);
	v[3] = LTVertex(p.x, 0.0f, p.z-r, m_current_color, 0.5f, 0.0f);
	v[4] = LTVertex(p.x+r, 0.0f, p.z-r, m_current_color, 1.0f, 0.0f);
	v[5] = LTVertex(p.x+r, 0.0f, p.z, m_current_color, 1.0f, 0.5f);
	v[6] = LTVertex(p.x+r, 0.0f, p.z+r, m_current_color, 1.0f, 1.0f);
	v[7] = LTVertex(p.x, 0.0f, p.z+r, m_current_color, 0.5f, 1.0f);
	v[8] = LTVertex(p.x-r, 0.0f, p.z+r, m_current_color, 0.0f, 1.0f);
	v[9] = LTVertex(p.x-r, 0.0f, p.z, m_current_color, 0.0f, 0.5f);
	for(int i = 0; i < 10; ++i)
	{
		v[i].y = m_hgtmap->GetHeight(v[i].x, v[i].z) + ybias;
	}
	m_vb->Unlock();
	m_vb->Set();
	ITexture* tex = (ITexture*)Core::GetMediaObject(moniker("selector_tex"));
	ITexture::MakeNull(m_device, 0);
	if(tex)
		tex->Set(0);
	m_device->SetTextureState(0, TS_COLOROP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	m_device->SetTextureState(0, TS_ALPHAOP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_ALPHAARG2, TA_DIFFUSE);
	
	m_device->DrawPrim(PT_TRIFAN, 0, 8);
	return OK;
}

void TOW_SelectIndicator::SetGameObject(TOW_GameObject* go)
{
	m_go = go;
}

void TOW_SelectIndicator::Release()
{
	if(m_vb)
	{
		m_vb->Release();
		delete m_vb;
		m_vb = NULL;
	}
	m_factory = NULL;
	m_device = NULL;

}

void TOW_SelectIndicator::SetHeightMap(IHeightMap* hgtmap)
{
	m_hgtmap = hgtmap;
}

void TOW_SelectIndicator::Color(unsigned long color)
{
	m_color = color;
	m_current_color = m_color;
}

unsigned long TOW_SelectIndicator::Color()
{
	return m_color;
}

bool TOW_SelectIndicator::Destroy(float t)
{
	dest_t = t;
	m_current_color = m_color;
	return true;
}

bool TOW_SelectIndicator::DestroyTick()
{
	if(Core::GetTime() - dest_t > 0.5f)
		return false;
	m_current_color = ColorInterpolate(m_color, m_color & 0x00ffffff, (Core::GetTime() - dest_t) / 0.5f);
	
	return true;
}