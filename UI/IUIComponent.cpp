#include "..\Common\IUIComponent.h"

IUIComponent::IUIComponent()
{

}

IUIComponent::~IUIComponent()
{

}

bool IUIComponent::HitTest(int x, int y)
{
	RECT r;
	r.left = m_pos.x;
	r.right = m_pos.x + m_size.cx;
	r.top = m_pos.y;
	r.bottom = m_pos.y + m_size.cy;
	POINT p = {x, y};
	return PtInRect(&r, p);
}


