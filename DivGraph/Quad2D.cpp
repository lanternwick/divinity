#include "Quad2D.h"

IDevice* Quad2D::m_device = 0;

Quad2D::Quad2D()
{
	m_vb = NULL;
}

Quad2D::~Quad2D()
{
	if(m_vb) delete m_vb;
	m_vb = NULL;
}

RESULT Quad2D::Render()
{
	if(!m_vb) return INVALIDDATA;
	RESULT r = m_vb->Set();
	if(RFAILED(r)) 
		return r;
	unsigned long fstate;
	m_device->SetRenderState(RS_ZWRITEENABLE, 0);
	m_device->GetRenderState(RS_FILLMODE, &fstate);
	//m_device->SetRenderState(RS_FILLMODE, FILL_SOLID);
	r = m_device->DrawPrim(PT_TRISTRIP, 0, 2);
	m_device->SetRenderState(RS_ZWRITEENABLE, 1);
	m_device->SetRenderState(RS_FILLMODE, fstate);
	return r;
}

RESULT Quad2D::Create(IDevice* device, IGraphObjFactory* factory, TLTVERTEX* quad_verts)
{
	RESULT r = factory->CreateVertexBuffer(4, sizeof(TLTVERTEX), USAGE_WRITEONLY, TLTVERTFVF, m_vb, NULL);
	if(RFAILED(r))
		return r;

	TLTVERTEX* v;
	r = m_vb->Lock(0, 0, (BYTE**)&v, 0);
	if(RFAILED(r))
		return r;

	v[0] = quad_verts[0];
	v[1] = quad_verts[1];
	v[2] = quad_verts[2];
	v[3] = quad_verts[3];
	

	m_vb->Unlock();

	m_device = device;
	return OK;
}
	




	

