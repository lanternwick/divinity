#include "UICursor.h"
#include "..\Common\singleton.h"
#include "UserInterface.h"
#include "..\Common\types.h"

UICursor::UICursor()
{
	m_device = NULL;
	m_vb = NULL;
	
}

UICursor::~UICursor()
{
	if(m_vb)
	{
		delete m_vb;
		m_vb = NULL;
	}
}

RESULT UICursor::Create(IDevice* device, IGraphObjFactory* factory)
{
	RESULT r;
	r = factory->CreateVertexBuffer(6, sizeof(TLTVERTEX), USAGE_DYNAMIC | USAGE_WRITEONLY, TLTVERTFVF, m_vb, NULL);
	m_device = device;
	
	return r;
}

RESULT UICursor::SetPos(POINT* p)
{
	pos = *p;
	return OK;
}

RESULT UICursor::SetCursor(moniker& cursor)
{
	m_cursor = cursor;
	return OK;
}

RESULT UICursor::Think()
{
	singleton<UserInterface*>::instance()->Input().GetMousePos(&pos);
	Update();
	return OK;
}

RESULT UICursor::Render()
{
	RESULT r;
	r = m_vb->Set();
	if(RFAILED(r)) return r;
	ITexture::MakeNull(m_device, 0);
	ITexture* t = (ITexture*)Core::GetMediaObject(m_cursor);
	if(t)
	{
		r = t->Set(0);
		if(RFAILED(r)) 
		{
			return r;
		}
	}
	
	m_device->SetTextureState(0, TS_COLOROP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	m_device->SetTextureState(0, TS_ALPHAOP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_ALPHAARG2, TA_DIFFUSE);
	

	r = m_device->DrawPrim(PT_TRILIST, 0, 2);

	return r;
}

void UICursor::Update()
{
	TLTVERTEX* p;
	m_vb->Lock(0, 0, (BYTE**)&p, LOCK_DISCARD);
	p[0] = TLTVertex(pos.x, pos.y, 0.1f, 1.0f, 0xffffffff, 0.0f, 0.0f);
	p[1] = TLTVertex(pos.x+20, pos.y, 0.1f, 1.0f, 0xffffffff, 1.0f, 0.0f);
	p[2] = TLTVertex(pos.x, pos.y+20, 0.1f, 1.0f, 0xffffffff, 0.0f, 1.0f);
	p[4] = p[1];
	p[3] = p[2];
	p[5] = TLTVertex(pos.x+20, pos.y+20, 0.1f, 1.0f, 0xffffffff, 1.0f, 1.0f);
	m_vb->Unlock();
	return;
}

bool UICursor::active()
{
	return false;
}