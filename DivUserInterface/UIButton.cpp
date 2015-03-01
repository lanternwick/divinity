#include "UIButton.h"

UIButton::UIButton()
{

}

UIButton::UIButton(char* id)
{
	m_id() = id;
}

RESULT UIButton::Create(IDevice* device, IGraphObjFactory* factory, float x, float y, float cx, float cy, unsigned long color, unsigned long caption_color, bool border, unsigned long border_color, float border_width, UIWindow* parent)
{
	return UIWindow::Create(device, factory, x, y, cx, cy, color, caption_color, false, border, 0, 0, border_width, border_color, 0, 0, parent);
}

RESULT UIButton::Render()
{
	UIWindow::Render();
	RECT re = {m_pos.x+3, m_pos.y + 3, m_pos.x + m_size.cx-3, m_pos.y + m_size.cy - 3 };
	return m_fontMan->DrawText(m_font, m_caption.c_str(), re, NULL, 1.0f, m_caption_color);
}

RESULT UIButton::Think()
{
	return OK;
}

bool UIButton::Mouse_ButtonUp(int button)
{
	return true;
}