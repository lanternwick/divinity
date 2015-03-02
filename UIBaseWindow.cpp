#include "UIBaseWindow.h"
#include "IInputDevice.h"

UIBaseWindow::UIBaseWindow()
{
	contains = NULL;
	m_dirty = false;
	m_window = NULL;
	m_texture = NULL;
	m_parent = NULL;
	m_focused = false;
}

UIBaseWindow::~UIBaseWindow()
{
	if(m_window)
	{
		delete m_window;
		m_window = NULL;
	}
	if(m_texture)
	{
		m_texture->Release();
	}
	m_texture = NULL;
	for(int i = 0; i < m_children.size(); ++i)
	{
		delete m_children[i];
	}
}

unsigned long UIBaseWindow::Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, 
								   ITexture* texture, UIWindow* parent)
{
	m_device = device;

	// multiply y pos and y size by WIDTH instead of height.  
	// this makes sure that equal x and y values LOOK the same on screen.
	if(!parent)
	{
		m_pos.x = xp*device->width();
		m_pos.y = yp*device->width(); 
		m_size.cx = cxp*device->width();
		m_size.cy = cyp*device->width();
	}
	else
	{
		m_pos.x = xp*parent->cx();
		m_pos.y = yp*parent->cx(); 
		m_size.cx = cxp*parent->cx();
		m_size.cy = cyp*parent->cx();
	}
	m_color = color;
	m_texture = texture;
	m_parent = parent;
	if(parent)
	{
		parent->AttachChildWindow(this);
	}
	Update();
	return 0;
}

RESULT UIBaseWindow::Render()
{
	if(!m_visible)
	{
		return OK;
	}
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
	for(int i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->Render();
	}
	return OK;
}

RESULT UIBaseWindow::Think()
{
	if(m_dirty)
	{
		Update();
	}
	for(int i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->Think();
	}
	return OK;
}

void UIBaseWindow::Update()
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
	for(int i = 0; i < m_children.size(); ++i)
	{
		m_children[i]->Update();
	}
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
	m_size.cx = (int)cxp;
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
	if((xp >= sx()) && (xp <= sx() + m_size.cx) &&
	   (yp >= sy()) && (yp <= sy() + m_size.cy))
	{
		return this;
	}
	return NULL;
}

UIWindow* UIBaseWindow::hit_test(float xp, float yp, float& cxp, float& cyp)
{
	if((xp >= sx()) && (xp <= sx() + m_size.cx) &&
	   (yp >= sy()) && (yp <= sy() + m_size.cy))
	   {
		cxp = xp - sx();
		cyp = yp - sy();
		return this;
	}
	return NULL;
}

int UIBaseWindow::AttachChildWindow(UIWindow* child)
{
	m_children.push_back(child);
	return 0;
}

int UIBaseWindow::Focus()
{
	m_focused = true;
	return 0;
}

int UIBaseWindow::Unfocus()
{
	m_focused = false;
	return 0;
}

float UIBaseWindow::sx()
{
	if(m_parent)
	{
		return m_parent->sx()+m_pos.x;
	}
	return m_pos.x;
}

float UIBaseWindow::sy()
{
	if(m_parent)
	{
		return m_parent->sy()+m_pos.y;
	}
	return m_pos.y;
}

float UIBaseWindow::scx()
{
	/*if(m_parent)
	{
		return m_parent->scx()+m_size.cx;
	}*/
	return m_size.cx;
}

float UIBaseWindow::scy()
{
	/*if(m_parent)
	{
		return m_parent->scy()+m_size.cy;
	}*/
	return m_size.cy;
}

bool UIBaseWindow::Keyboard_KeyUp(IInputDevice* input, char key)
{
	return true;
}

bool UIBaseWindow::Keyboard_KeyDown(IInputDevice* input, char key)
{
	if(contains)
	{
		return contains->Keyboard_KeyDown(input, key);
	}
	return true;
}

bool UIBaseWindow::Mouse_ButtonUp(IInputDevice* input, int button)
{
	
	UIWindow* win;
	POINT p;
	input->GetMousePos(&p);
	if(m_children.size())
	{
		for(int i = 0; i < m_children.size(); ++i)
		{
			win = m_children[i]->hit_test(p.x, p.y);
			if(win)
			{
				win->Mouse_ButtonUp(input, button);
				return true;
			}
		}
	}
	return false;
}

bool UIBaseWindow::Mouse_ButtonDown(IInputDevice* input, int button)
{
	UIWindow* win;
	POINT p;
	input->GetMousePos(&p);
	if(m_children.size())
	{
		for(int i = 0; i < m_children.size(); ++i)
		{
			win = m_children[i]->hit_test(p.x, p.y);
			if(win)
			{
				win->Mouse_ButtonDown(input, button);
				return true;
			}
		}
	}
	return false;
}

bool UIBaseWindow::Mouse_Move(IInputDevice* input, POINT p, int mb1, int mb2, int mb3, int mb4)
{
	if(contains)
	{
		return contains->Mouse_Move(input, p, mb1, mb2, mb3, mb4);
	}
	return true;
}

bool UIBaseWindow::Mouse_Scroll(IInputDevice* input, long scroll)
{
	if(contains)
	{
		return contains->Mouse_Scroll(input, scroll);
	}
	return true;
}