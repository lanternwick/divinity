#include "UIButton.h"
#include "IInputDevice.h"

UIButton::UIButton()
{
	m_font = NULL;
	m_down_on_me = NULL;
	
}

UIButton::~UIButton()
{

}

int UIButton::Create(IDevice* device, float xp, float yp, float cxp, float cyp, ITexture* texture, unsigned long color, 
		unsigned long color_d, unsigned long t_color, Font* fnt, const std::string& text, const std::string& command, 
		UIWindow* parent)
{
	m_button_cmd = command;
	m_button_text = text;
	m_font = fnt;
	m_color_d = color_d;
	m_t_color = t_color;
	UIBaseWindow::Create(device, xp, yp, cxp, cyp, color, texture, parent);
	return 0;
}

bool UIButton::Mouse_ButtonDown(IInputDevice* input, int button)
{
	m_down_on_me = true;
	m_dirty = true;
	m_input = input;
	return false;
}

bool UIButton::Mouse_ButtonUp(IInputDevice* input, int button)
{
	if(!m_down_on_me)
	{
		return false;
	}
	m_down_on_me = false;
	m_dirty = true;
	return false;
}

RESULT UIButton::Think()
{
	POINT p;
	if(m_down_on_me)	
	{
		m_input->GetMousePos(&p);
		if((p.x <= sx()) || (p.x >= sx()+m_size.cx) || (p.y <= sy()) || (p.y >= sy()+m_size.cy))
		{
			m_down_on_me = false;
			m_dirty = true;
		}
	}
	
	return UIBaseWindow::Think();
}

RESULT UIButton::Render()
{
	UIBaseWindow::Render();
	RECT r = {sx(), sy(), sx() + m_size.cx, sy() + m_size.cy};
	if(m_font)
	{
		m_font->DrawText(m_button_text.c_str(), r, "xx", 1.0f, m_t_color);
	}
	return OK;
}

void UIButton::Update()
{
	TLTVERTEX verts[4];
	unsigned long color;
	if(m_down_on_me)
	{
		color = m_color_d;
	}
	else
	{
		color = m_color;
	}

	if(m_parent)
	{
		verts[0] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y, 0.0f, 1.0f, color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y, 0.0f, 1.0f, color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cx, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, color, 1.0f, 1.0f);
	}
	else
	{
		verts[0] = TLTVertex(m_pos.x, m_pos.y, 0.0f, 1.0f, color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_pos.x+m_size.cx, m_pos.y, 0.0f, 1.0f, color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_pos.x, m_pos.y+m_size.cy, 0.0f, 1.0f, color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+m_size.cy, 0.0f, 1.0f, color, 1.0f, 1.0f);
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