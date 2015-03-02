#include "UIScrollbar.h"
#include "IInputDevice.h"

UIScrollbar::UIScrollbar()
{
	m_arrow_tex = NULL;
	m_thumb_tex = NULL;
	m_thumb = NULL;
	m_up = NULL;
	m_dn = NULL;
	m_min = 0;
	m_max = 0;
	m_val = 0;
	m_attached = NULL;
}

UIScrollbar::~UIScrollbar()
{
	if(m_arrow_tex)
	{
		m_arrow_tex->Release();
	}
	if(m_thumb_tex)
	{
		m_thumb_tex->Release();
	}
	m_arrow_tex = m_thumb_tex = NULL;
	if(m_thumb)
	{
		delete m_thumb;
	}
	if(m_up)
	{
		delete m_up;
	}
	if(m_dn)
	{
		delete m_dn;
	}
	m_up = m_dn = m_thumb = NULL;
	m_attached = NULL;
}

int UIScrollbar::Create(IDevice* device, float xp, float yp, float cxp, float cyp, ITexture* text, ITexture* arrow,
						ITexture* thumb, unsigned long color, int min, int max, UIWindow* parent)
{
	m_arrow_tex = arrow;
	m_thumb_tex = thumb;
	m_min = min;
	m_max = max;
	UIBaseWindow::Create(device, xp, yp, cxp, cyp, color, text, parent);
	
	Update();
	return 0;
}

bool UIScrollbar::Mouse_ButtonDown(IInputDevice* input, int button)
{

	return false;
}

bool UIScrollbar::Mouse_ButtonUp(IInputDevice* input, int button)
{
	POINT p;
	window_message msg;
	input->GetMousePos(&p);

	if((p.x >= sx()) && (p.x <= sx() + scx()) && (p.y >= sy()) && (p.y <= sy() + scx()))
	{
		// hit up button
		if(m_val != m_min)
		{
			--m_val;
			if(m_attached)
			{
				msg.message = SCROLLUP;
				m_attached->send_message(msg);
			}
			m_dirty = true;
			return false;
		}
	}

	if((p.x >= sx()) && (p.x <= sx() + scx()) && (p.y >= sy()+scy()-scx()) && (p.y <= sy() + scy()))
	{
		// hit dn button
		if(m_val != m_max)
		{
			++m_val;
			m_dirty = true;
			if(m_attached)
			{
				msg.message = SCROLLDN;
				m_attached->send_message(msg);
			}
			return false;
		}
	}

	float d = sy()+m_size.cx;
	float si = m_size.cy-2*m_size.cx;
	float s = (p.y-d) / si;
	if((m_val / float((m_max+1) - m_min)) > s)
	{
		if(m_val != m_min)
		{
			--m_val;
			if(m_attached)
			{
				msg.message = SCROLLUP;
				m_attached->send_message(msg);
			}
			m_dirty = true;
			return false;
		}
	}
	else
	{
		if(m_val != m_max)
		{
			++m_val;
			if(m_attached)
			{
				msg.message = SCROLLDN;
				m_attached->send_message(msg);
			}
			m_dirty = true;
			return false;
		}
	}
	
	
	return false;
}

RESULT UIScrollbar::Render()
{
	if(!m_visible)
	{
		return OK;
	}
	m_device->SetTextureState(0, TS_COLOROP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	m_device->SetTextureState(0, TS_ALPHAOP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_ALPHAARG2, TA_DIFFUSE);
	m_device->SetTextureState(0, TS_MAGFILTER, 0);
	m_device->SetTextureState(0, TS_MINFILTER, 0);
	if(m_window)
	{
		
		if(m_texture)
		{
			m_texture->Set(0);
		}
		else
		{
			ITexture::MakeNull(m_device, 0);
		}
		
		m_window->Render();
	}

	if(m_up)
	{
		
		if(m_arrow_tex)
		{
			m_arrow_tex->Set(0);
		}
		else
		{
			ITexture::MakeNull(m_device, 0);
		}
		
		m_up->Render();
	}
	
	if(m_dn)
	{
		
		if(m_arrow_tex)
		{
			m_arrow_tex->Set(0);
		}
		else
		{
			ITexture::MakeNull(m_device, 0);
		}
		
		m_dn->Render();
	}

	if(m_thumb)
	{
		
		if(m_thumb_tex)
		{
			m_thumb_tex->Set(0);
		}
		else
		{
			ITexture::MakeNull(m_device, 0);
		}
		
		m_thumb->Render();
	}
	
	return OK;
}

RESULT UIScrollbar::Think()
{
	if(m_dirty)
	{
		Update();
	}
	return OK;
}

void UIScrollbar::Update()
{
	TLTVERTEX verts[4];
	if(m_parent)
	{
		verts[0] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
	}
	else
	{
		verts[0] = TLTVertex(m_pos.x, m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_pos.x+m_size.cx, m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_pos.x, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
	}

	if(m_window)
	{
		delete m_window;
	}
	m_window = new Quad2D;
	m_window->Create(m_device, verts);
	
	// up arrow
	if(m_parent)
	{
		verts[0] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+m_size.cx, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y+m_size.cx, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
	}
	else
	{
		verts[0] = TLTVertex(m_pos.x, m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_pos.x+m_size.cx, m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_pos.x, m_pos.y+m_size.cx, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+m_size.cx, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
	}

	if(m_up)
	{
		delete m_up;
	}
	m_up = new Quad2D;
	m_up->Create(m_device, verts);

	// dn arrow
	if(m_parent)
	{
		verts[0] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+m_size.cy-m_size.cx, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[1] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y+m_size.cy-m_size.cx, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
		verts[2] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[3] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
	}
	else
	{
		verts[0] = TLTVertex(m_pos.x, m_pos.y+m_size.cy-m_size.cx, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[1] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+m_size.cy-m_size.cx, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
		verts[2] = TLTVertex(m_pos.x, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[3] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
	}

	if(m_dn)
	{
		delete m_dn;
	}
	m_dn = new Quad2D;
	m_dn->Create(m_device, verts);

	// thumb
	int d = (m_max+1) - m_min;
	float s = float(m_val) / d;
	
	float t_len = m_size.cy - 2*m_size.cx;
	float cyt = t_len / (d);
	float yt = m_size.cx + t_len*s;
	if(m_parent)
	{
		verts[0] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+yt, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y+yt, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+cyt+yt, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y+cyt+yt, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
	}
	else
	{
		verts[0] = TLTVertex(m_pos.x, m_pos.y+yt, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+yt, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_pos.x, m_pos.y+cyt+yt, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+cyt+yt, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
	}

	if(m_thumb)
	{
		delete m_thumb;
	}
	m_thumb = new Quad2D;
	m_thumb->Create(m_device, verts);

}

int UIScrollbar::value()
{
	return m_val;
}

void UIScrollbar::value(int n)
{
	window_message msg;
	if((n > m_max) || (n < m_min))
	{
		return;
	}
	m_val = n;
	if(m_attached)
	{
		msg.message = SBVALCNG;
		msg.ulong_in[0] = m_val;
		m_attached->send_message(msg);
	}
	m_dirty = true;
}

void UIScrollbar::AttachTo(UIWindow* win)
{
	m_attached = win;
}