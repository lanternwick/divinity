#include "UIBaseWindow.h"

UIBaseWindow::UIBaseWindow()
{
	contains = NULL;
	m_dirty = false;
	m_window = NULL;
}

UIBaseWindow::~UIBaseWindow()
{
	if(m_window)
	{
		delete m_window;
		m_window = NULL;
	}
}

unsigned long UIBaseWindow::Create(IDevice* device, IGraphObjFactory* factory, float xp, float yp, float cxp, float cyp, unsigned long color, moniker& texture, UIWindow* parent)
{
	m_device = device;
	m_factory = factory;

	// multiply y pos and y size by WIDTH instead of height.  
	// this makes sure that equal x and y values LOOK the same on screen.
	m_pos.x = xp*device->width();
	m_pos.y = yp*device->width(); 
	m_size.cx = cxp*device->width();
	m_size.cy = cyp*device->width();
	m_color = color;
	m_texture = texture;
	Update();
	return 0;
}

RESULT UIBaseWindow::Render()
{
	ITexture* tex;
	if(!m_visible)
	{
		return OK;
	}
	if(m_window)
	{
		tex = (ITexture*)Core::GetMediaObject(m_texture);
		if(tex)
		{
			tex->Set(0);
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
		m_window->Render();
	}
	return OK;
}

RESULT UIBaseWindow::Think()
{
	if(m_dirty)
	{
		Update();
	}
	return OK;
}

void UIBaseWindow::Update()
{
	TLTVERTEX verts[4];
	verts[0] = TLTVertex(m_pos.x, m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
	verts[1] = TLTVertex(m_pos.x+m_size.cx, m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
	verts[2] = TLTVertex(m_pos.x, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
	verts[3] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 1.0f);

	if(m_window)
	{
		delete m_window;
	}
	m_window = new Quad2D;
	m_window->Create(m_device, m_factory, verts);
}

float UIBaseWindow::x()
{
	return m_pos.x;
}

float UIBaseWindow::y()
{
	return m_pos.y;
}

float UIBaseWindow::cx()
{
	return m_size.cx;
}

float UIBaseWindow::cy()
{
	return m_size.cy;
}

void UIBaseWindow::x(float xp)
{
	m_pos.x = xp;
	m_dirty = true;
}

void UIBaseWindow::y(float yp)
{
	m_pos.y = yp;
	m_dirty = true;
}

void UIBaseWindow::cx(float cxp)
{
	m_size.cx= (int)cxp;
	m_dirty = true;
}

void UIBaseWindow::cy(float cyp)
{
	m_size.cy = (int)cyp;
	m_dirty = true;
}

void UIBaseWindow::send_message(window_message& msg)
{
	
}

bool UIBaseWindow::active()
{
	return m_active;
}

bool UIBaseWindow::visible()
{
	return m_visible;
}

void UIBaseWindow::visible(bool v)
{
	m_visible = v;
}

UIWindow* UIBaseWindow::hit_test(float xp, float yp)
{
	if((xp >= m_pos.x) && (xp <= m_pos.x + m_size.cx) &&
	   (yp >= m_pos.y) && (yp <= m_pos.y + m_size.cy))
	   {
		return this;
	}
	return NULL;
}

UIWindow* UIBaseWindow::hit_test(float xp, float yp, float& cxp, float& cyp)
{
	if((xp <= m_pos.x) && (xp >= m_pos.x + m_size.cx) &&
	   (yp <= m_pos.y) && (yp >= m_pos.y + m_size.cy))
	   {
		cxp = xp - m_pos.x;
		cyp = yp - m_pos.y;
		return this;
	}
	return NULL;
}