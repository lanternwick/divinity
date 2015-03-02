#include "Line3D.h"

Line3D::Line3D()
{
	m_device = NULL;
	m_vb = NULL;
}

Line3D::Line3D(VECTOR* s, VECTOR* e)
{
	m_device = NULL;
	m_vb = NULL;
	m_s = *s;
	m_e = *e;
}

Line3D::~Line3D()
{
	Release();
}

RESULT Line3D::Create(IDevice* device, IGraphObjFactory* factory)
{
	m_device = device;
	m_factory = factory;
	factory->CreateVertexBuffer(4, sizeof(LTVERTEX), USAGE_DYNAMIC, LTVERTFVF, m_vb, NULL);
	return OK;
}

RESULT Line3D::Render()
{
	float cx, cy, cz;
	VECTOR verts[8];
	cx = (m_s.x + m_e.x) / 2;
	cy = (m_s.y + m_e.y) / 2;
	cz = (m_s.z + m_e.z) / 2;

	float half_wid = m_width / 2.0f;
	
	VECTOR v = m_e - m_s;
	VectorNormalize(&v, &v);
	float te = max(max(v.x, v.y), v.z);
	float mi = min(min(v.x, v.y), v.z);
	if(te == v.x)
	{
		if(mi == v.y)
		{
			v.y = te;
			v.x = mi;
		}
		else
		{
			v.z = te;
			v.x = mi;
		}
	}
	if(te == v.y)
	{
		if(mi == v.z)
		{
			v.z = te;
			v.y = mi;
		}
		else
		{
			v.x = te;
			v.y = mi;
		}
	}
	if(te == v.z)
	{
		if(mi == v.x)
		{
			v.x = te;
			v.z = mi;
		}
		else
		{
			v.y = te;
			v.z = mi;
		}
	}
	//v.y = v.z;
	//v.z = te;
	//te = v.x;
	//v.x = v.z;
	//v.z = te;
	v.x *= -half_wid;
	v.y *= -half_wid;
	v.z *= -half_wid;

	verts[0] = VECTOR(m_s.x-cx-v.x, m_s.y - cy+v.y, m_s.z - cz-v.z); //, 0xffffffff, 0.0f, 0.0f);
	verts[1] = VECTOR(m_s.x-cx+v.x, m_s.y - cy-v.y, m_s.z - cz+v.z); //, 0xffffffff, 0.0f, 1.0f);

	verts[2] = VECTOR(m_e.x-cx-v.x, m_e.y - cy+v.y, m_e.z - cz-v.z); //, 0xffffffff, 1.0f, 0.0f);
	verts[3] = VECTOR(m_e.x-cx+v.x, m_e.y - cy-v.y, m_e.z - cz+v.z); //, 0xffffffff, 1.0f, 1.0f);

	

	MATRIX m, t;
	m_device->GetTransform(T_VIEW, &m);
	MatrixMatrixBillboardTranspose(&t, &m, cx, cy, cz);
	LTVERTEX* vp;
	m_vb->Lock(0, 0, (BYTE**)&vp, LOCK_DISCARD);
	MatrixMatrixVectorApply(&verts[4], &t, &verts[0]);
	MatrixMatrixVectorApply(&verts[5], &t, &verts[1]);
	MatrixMatrixVectorApply(&verts[6], &t, &verts[2]);
	MatrixMatrixVectorApply(&verts[7], &t, &verts[3]);
	vp[0] = LTVertex(verts[4].x, verts[4].y, verts[4].z, m_color, 0.0f, 0.0f);
	vp[1] = LTVertex(verts[5].x, verts[5].y, verts[5].z, m_color, 0.0f, 1.0f);
	vp[2] = LTVertex(verts[6].x, verts[6].y, verts[6].z, m_color, 1.0f, 0.0f);
	vp[3] = LTVertex(verts[7].x, verts[7].y, verts[7].z, m_color, 1.0f, 1.0f);
	m_vb->Unlock();
	m_vb->Set();
	ITexture* lt = (ITexture*)Core::GetMediaObject(moniker("line_tex"));
	lt->Set(0);
	//render_state<RS_ZWRITEENABLE, 0, true> rs(m_device);
	m_device->DrawPrim(PT_TRISTRIP, 0, 2);
	return OK;
}

void Line3D::Start(VECTOR* s)
{
	m_s = *s;
}

void Line3D::End(VECTOR* e)
{
	m_e = *e;
}

VECTOR Line3D::Start()
{
	return m_s;
}

VECTOR Line3D::End()
{
	return m_e;
}

void Line3D::Width(float wid)
{
	m_width = wid;
}

float Line3D::Width()
{
	return m_width;
}

void Line3D::Color(unsigned long color)
{
	m_color = color;
}

unsigned long Line3D::Color()
{
	return m_color;
}

void Line3D::Regenerate()
{
	
}

void Line3D::Release()
{
	if(m_vb)
	{
		m_vb->Release();
		delete m_vb;
	}
	m_vb = NULL;
}