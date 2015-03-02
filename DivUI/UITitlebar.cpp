#include "UITitlebar.h"

UITitlebar::UITitlebar()
{
	m_bar = NULL;
	contains = NULL;
	m_dirty = false;
}

UITitlebar::~UITitlebar()
{
	if(m_bar)
	{
		delete m_bar;
		m_bar = NULL;
	}
	contains = NULL;
}

unsigned long UITitlebar::Create(IDevice* device, IGraphObjFactory* factory, int cyp, unsigned long color, moniker& texture, UIWindow* wrap)
{
	m_device = device;
	m_factory = factory;
	m_y = cyp;
	m_color = color;
	contains = wrap;
	m_texture = texture;
	Update();
	return 0;
}

RESULT UITitlebar::Render()
{
	ITexture* tex;
	if(m_bar)
	{
		tex = (ITexture*)Core::GetMediaObject(m_texture);
		if(tex)
		{
			tex->Set(0);
		}
		else
		{
			ITexture::MakeNull(m_device, 1);
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
	return OK;
}

void UITitlebar::Update()
{
	TLTVERTEX verts[4];
	verts[0] = TLTVertex(contains->x(), contains->y()-m_y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
	verts[1] = TLTVertex(contains->x()+contains->cx(), contains->y()-m_y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
	verts[2] = TLTVertex(contains->x(), contains->y(), 0.0f, 1.0f, m_color, 0.0f, 1.0f);
	verts[3] = TLTVertex(contains->x()+contains->cx(), contains->y(), 0.0f, 1.0f, m_color, 1.0f, 1.0f);

	if(m_bar)
	{
		delete m_bar;
	}
	m_bar = new Quad2D;
	m_bar->Create(m_device, m_factory, verts);
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
	throw;
}

void UITitlebar::x(float xp)
{
	if(contains)
	{
		contains->x(xp);
		m_dirty = true;
	}
	throw;
}

void UITitlebar::y(float yp)
{
	if(contains)
	{
		contains->y(yp);
		m_dirty = true;
	}
}

void UITitlebar::cx(float cxp)
{
	if(contains)
	{
		contains->cx(cxp);
		m_dirty = true;
	}
}

void UITitlebar::cy(float cyp)
{
	if(contains)
	{
		contains->cy(cyp);
		m_dirty = true;
	}
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
	if((xp >= contains->x()) && (xp <= contains->x() + contains->cx()) &&
	   (yp >= contains->y() + m_y) && (yp <= contains->y()))
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
	   (yp >= contains->y() + m_y) && (yp <= contains->y()))
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