#include "UICursor.h"

UICursor::UICursor()
{
	m_device = NULL;
	m_vb = NULL;
	cursor_info = "TOW";
}

UICursor::~UICursor()
{
	if(m_vb)
	{
		delete m_vb;
		m_vb = NULL;
	}
}

RESULT UICursor::Create(IDevice* device, IGraphObjFactory* factory, FontManager* fm)
{
	RESULT r;
	r = factory->CreateVertexBuffer(6, sizeof(TLTVERTEX), USAGE_DYNAMIC | USAGE_WRITEONLY, TLTVERTFVF, m_vb, "cursor vb");
	m_device = device;
	m_fontMan = fm;
	return r;
}

RESULT UICursor::SetPos(POINT* p)
{
	pos = *p;
	return OK;
}

RESULT UICursor::SetCursor(ITexture* cursor)
{
	m_cursor = cursor;
	return OK;
}

RESULT UICursor::Think()
{
	Update();
	return OK;
}

RESULT UICursor::Render()
{
	RESULT r;
	r = m_vb->Set();
	if(RFAILED(r)) return r;
	ITexture::MakeNull(m_device, 0);
	if(m_cursor)
	{
		r = m_cursor->Set(0);
		if(RFAILED(r)) return r;
	}
	
	r = m_device->DrawPrim(PT_TRILIST, 0, 2);
	RECT rc = {pos.x + 8, pos.y - 7, pos.x+125, pos.y+125};
	m_fontMan->DrawText("default", cursor_info.c_str(), rc, NULL, 1.0f, 0xffffffff);
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

RESULT UICursor::SetCursorInfo(std::string& ci)
{
	cursor_info = ci;
	return OK;
}