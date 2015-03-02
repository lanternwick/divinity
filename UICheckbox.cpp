#include "UICheckbox.h"

UICheckbox::UICheckbox()
{
	m_val = false;
	m_checked_tex = NULL;
}

UICheckbox::~UICheckbox()
{
	if(m_checked_tex)
	{
		m_checked_tex->Release();
	}
	if(m_unchecked_tex)
	{
		m_unchecked_tex->Release();
	}
	m_unchecked_tex = NULL;
	m_checked_tex = NULL;
	m_texture = NULL;

}

int UICheckbox::Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, 
					   unsigned long t_color, Font* fnt, ITexture* checked, ITexture* unchecked, 
					   const std::string& text, UIWindow* parent)
{
	UIBaseWindow::Create(device, xp, yp, cxp, cyp, color, unchecked, parent);
	m_checked_tex = checked;
	m_unchecked_tex = unchecked;
	m_t_color = t_color;
	m_font = fnt;
	m_text = text;
	return 0;
}

bool UICheckbox::Mouse_ButtonUp(IInputDevice* input, int button)
{
	m_val = !m_val;
	if(m_val)
	{
		m_texture = m_checked_tex;
	}
	else
	{
		m_texture = m_unchecked_tex;
	}
	return false;
}

RESULT UICheckbox::Render()
{
	UIBaseWindow::Render();
	RECT r = { sx()+m_size.cy, sy(), sx()+m_size.cx, sy()+m_size.cy};
	if(m_font)
	{
		m_font->DrawText(m_text.c_str(), r, NULL, float(m_size.cy-4) / float(m_font->GetCellSize().cy), m_t_color);
	}
	return OK;
}

void UICheckbox::Update()
{
	TLTVERTEX verts[4];
	if(m_parent)
	{
		verts[0] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cy, m_parent->sy()+m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_parent->sx()+m_pos.x, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_parent->sx()+m_pos.x+m_size.cy, m_parent->sy()+m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
	}
	else
	{
		verts[0] = TLTVertex(m_pos.x, m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
		verts[1] = TLTVertex(m_pos.x+m_size.cy, m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
		verts[2] = TLTVertex(m_pos.x, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
		verts[3] = TLTVertex(m_pos.x+m_size.cy, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 1.0f);
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