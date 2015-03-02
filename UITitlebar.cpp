#include "UITitlebar.h"
#include "IInputDevice.h"

UITitlebar::UITitlebar()
{
	m_bar = NULL;
	contains = NULL;
	m_dirty = false;
	m_texture = NULL;
	m_font = NULL;
	m_attachedtomouse = false;
	m_focused = false;
}

UITitlebar::~UITitlebar()
{
	if(m_bar)
	{
		delete m_bar;
		m_bar = NULL;
	}
	if(m_texture)
	{
		m_texture->Release();
	}
	m_texture = NULL;
	if(contains)
	{
		delete contains;
	}
	contains = NULL;
	m_font = NULL;
}

unsigned long UITitlebar::Create(IDevice* device, int cyp, unsigned long color, unsigned long inactive_color, ITexture* texture, Font* fnt, UIWindow* wrap)
{
	m_device = device;
	m_y = cyp;
	m_color = color;
	m_icolor = inactive_color;
	contains = wrap;
	m_texture = texture;
	m_font = fnt;
	Update();
	return 0;
}

RESULT UITitlebar::Render()
{
	if(m_bar)
	{
		if(m_texture)
		{
			m_texture->Set(0);
		}
		else
		{
			ITexture::MakeNull(m_device, 0);
		}
		m_device->SetTextureState(0, TS_COLOROP, TOP_MODULATE);
		m_device->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
		m_device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
		m_device->SetTextureState(0, TS_ALPHAOP, TOP_MODULATE);
		m_device->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);
		m_device->SetTextureState(0, TS_ALPHAARG2, TA_DIFFUSE);
		m_device->SetTextureState(0, TS_MAGFILTER, 0);
		m_device->SetTextureState(0, TS_MINFILTER, 0);
		m_bar->Render();
		if(m_font)
		{
			RECT r = { contains->x()+3, contains->y() - m_y, contains->x() + contains->cx(), contains->y() };
			m_font->DrawText(contains->window_name()().c_str(), r, NULL, 1.0f, 0xff000000);
		}

	}
	if(contains)
	{
		contains->Render();
	}
	return OK;
}

RESULT UITitlebar::Think()
{
	if(m_dirty)
	{
		Update();
	}
	if(contains)
	{
		contains->Think();
	}
	return OK;
}

void UITitlebar::Update()
{
	TLTVERTEX verts[4];
	unsigned long color;
	if(m_focused) 
	{
		color = m_color;
	}
	else
	{
		color = m_icolor;
	}
	verts[0] = TLTVertex(contains->x(), contains->y()-m_y, 0.1f, 1.0f, color, 0.0f, 0.0f);
	verts[1] = TLTVertex(contains->x()+contains->cx(), contains->y()-m_y, 0.1f, 1.0f, color, 1.0f, 0.0f);
	verts[2] = TLTVertex(contains->x(), contains->y(), 0.1f, 1.0f, color, 0.0f, 1.0f);
	verts[3] = TLTVertex(contains->x()+contains->cx(), contains->y(), 0.1f, 1.0f, color, 1.0f, 1.0f);

	if(m_bar)
	{
		delete m_bar;
	}
	m_bar = new Quad2D;
	m_bar->Create(m_device, verts);
}

float UITitlebar::x()
{
	if(contains)
	{
		return contains->x();
	}
	throw;
}

float UITitlebar::y()
{
	if(contains)
	{
		return contains->y();
	}
	throw;
}

float UITitlebar::cx()
{
	if(contains)
	{
		return contains->cx();
	}
	throw;
}

float UITitlebar::cy()
{
	if(contains)
	{
		return contains->cy();
	}
	else
	{
		throw;
	}
}

void UITitlebar::x(float xp)
{
	if(contains)
	{
		contains->x(xp);
		m_dirty = true;
		return;
	}
	else
	{
		throw;
	}
}

void UITitlebar::y(float yp)
{
	if(contains)
	{
		contains->y(yp);
		m_dirty = true;
		return;
	}
}

void UITitlebar::cx(float cxp)
{
	if(contains)
	{
		contains->cx(cxp);
		m_dirty = true;
		return;
	}
}

void UITitlebar::cy(float cyp)
{
	if(contains)
	{
		contains->cy(cyp);
		m_dirty = true;
		return;
	}
}

bool UITitlebar::Mouse_ButtonUp(IInputDevice* input, int button)
{
	POINT p;
	input->GetMousePos(&p);
	float xp = p.x;
	float yp = p.y;
	if((xp >= contains->x()) && (xp <= (contains->x() + contains->cx())) &&
	   (yp >= contains->y() - m_y) && (yp <= contains->y()))
	{
		DetachFromMouse();
	}
	else
	{
		contains->Mouse_ButtonUp(input, button);
	}
	return false;
}

bool UITitlebar::Mouse_ButtonDown(IInputDevice* input, int button)
{
	POINT p;
	float rx;
	float ry;
	input->GetMousePos(&p);
	float xp = p.x;
	float yp = p.y;
	if((xp >= contains->x()) && (xp <= (contains->x() + contains->cx())) &&
	   (yp >= contains->y() - m_y) && (yp <= contains->y()))
	{
		rx = xp - contains->x();
		ry = yp - contains->y();
		AttachToMouse(rx, ry);
	}
	else
	{
		contains->Mouse_ButtonDown(input, button);
	}
	return false;
}

void UITitlebar::send_message(window_message& msg)
{
	if(contains)
	{
		contains->send_message(msg);
	}
}

int UITitlebar::titlebar_y() const
{
	return m_y;
}

void UITitlebar::titlebar_y(int tby)
{
	m_y = tby;
	m_dirty = true;
}

UIWindow* UITitlebar::hit_test(float xp, float yp)
{
	if(!contains)
	{
		return NULL;
	}
	if((xp >= contains->x()) && (xp <= (contains->x() + contains->cx())) &&
	   (yp >= contains->y() - m_y) && (yp <= contains->y()))
	{
		return this;
	}
	if(contains->hit_test(xp, yp))
	{
		return this;
	}
	return NULL;
}

UIWindow* UITitlebar::hit_test(float xp, float yp, float& cxp, float& cyp)
{
	if(!contains)
	{
		return NULL;
	}
	if((xp >= contains->x()) && (xp <= contains->x() + contains->cx()) &&
	   (yp >= contains->y() - m_y) && (yp <= contains->y()))
	{
		cxp = xp - contains->x();
		cyp = yp - (contains->y() - m_y);
		return this;
	}
	if(contains->hit_test(xp, yp, cxp, cyp))
	{
		return this;
	}
	return NULL;
}

bool UITitlebar::AttachToMouse(int offsetx, int offsety)
{
	m_moffsetx = offsetx;
	m_moffsety = offsety;
	m_attachedtomouse = true;
	return true;
}

bool UITitlebar::DetachFromMouse()
{
	m_attachedtomouse = false;
	return true;
}

int UITitlebar::GetMouseOffset(int& offsetx, int& offsety)
{
	offsetx = m_moffsetx;
	offsety = m_moffsety;
	return 0;
}

bool UITitlebar::IsAttachedToMouse()
{
	return m_attachedtomouse;
}

int UITitlebar::Focus()
{
	m_focused = true;
	m_dirty = true;
	if(contains)
	{
		contains->Focus();
	}
	return 0;
}

int UITitlebar::Unfocus()
{
	m_focused = false;
	m_dirty = true;
	if(contains)
	{
		contains->Unfocus();
	}
	return 0;
}