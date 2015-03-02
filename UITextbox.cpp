#include "UITextbox.h"
#include "IInputDevice.h"
#include "KeyConstants.h"

UITextbox::UITextbox()
{
	m_font = NULL;
	m_attached = NULL;
}

UITextbox::~UITextbox()
{
	m_font = NULL;
	m_attached = NULL;
}

int UITextbox::Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, unsigned long t_color,
					  ITexture* texture, Font* fnt, UIWindow* parent)
{
	UIBaseWindow::Create(device, xp, yp, cxp, cyp, color, texture, parent);
	m_font = fnt;
	m_t_color = t_color;
	return 0;
}

bool UITextbox::Keyboard_KeyDown(IInputDevice* input, char key)
{
	bool shift = (input->GetKey(DIV_KEY_RSHIFT) | input->GetKey(DIV_KEY_LSHIFT));
	switch(key)
	{
	case DIV_KEY_RETURN:
	case DIV_KEY_RSHIFT:
	case DIV_KEY_LSHIFT:
		break;
	default:
		char c = input->KeyToChar(key, shift);
		if(c == '\b')
		{
			if(m_text.length())
			{
				m_text.resize(m_text.length()-1);
			}
			break;
		}
		m_text += c;
	}
	return false;
}

bool UITextbox::Keyboard_KeyUp(IInputDevice* input, char key)
{
	return false;
}

RESULT UITextbox::Render()
{
	UIBaseWindow::Render();
	RECT r = {sx(), sy(), sx()+m_size.cx, sy()+m_size.cy};
	int csy;
	if(m_font)
	{
		std::string t(m_text);
		if(m_focused)
		{
			t+='|';
		}
		csy = m_font->GetCellSize().cy;

		m_font->DrawText(t.c_str(), r, NULL, float(m_size.cy-4) / float(csy), m_t_color);
	}
	return OK;
}

void UITextbox::AttachTo(UIWindow* win)
{
	m_attached = win;
}