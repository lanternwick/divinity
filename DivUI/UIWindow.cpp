#include "UIWindow.h"

UIWindow::UIWindow()
{
	contains = NULL;
}

UIWindow::~UIWindow()
{

}

float UIWindow::x()
{
	if(contains)
	{
		return contains->x();
	}
	throw;
}

float UIWindow::y()
{
	if(contains)
	{
		return contains->y();
	}
	throw;
}

float UIWindow::cx()
{
	if(contains)
	{
		return contains->cx();
	}
	throw;
}

float UIWindow::cy()
{
	if(contains)
	{
		return contains->cy();
	}
	throw;
}

void UIWindow::x(float xp)
{
	if(contains)
	{
		contains->x(xp);
	}
}

void UIWindow::y(float yp)
{
	if(contains)
	{
		contains->y(yp);
	}
}

void UIWindow::cx(float cxp)
{
	if(contains)
	{
		contains->cx(cxp);
	}
}

void UIWindow::cy(float cyp)
{
	if(contains)
	{
		contains->cy(cyp);
	}
}

void UIWindow::send_message(window_message& msg)
{
	if(contains)
	{
		contains->send_message(msg);
	}
}

bool UIWindow::active()
{
	if(contains)
	{
		return contains->active();
	}
	return false;
}

const moniker& UIWindow::window_name() const
{
	if(contains)
	{
		return contains->window_name();
	}
	return m_window_name;
	
}

UIWindow* UIWindow::hit_test(float xp, float yp)
{
	if(contains)
	{
		if(contains->hit_test(xp, yp))
		{
			return this;
		}
	}
	return NULL;
}

UIWindow* UIWindow::hit_test(float xp, float yp, float& cxp, float& cyp)
{
	if(contains)
	{
		if(contains->hit_test(xp, yp, cxp, cyp))
		{
			return this;
		}
	}
	return NULL;

}

bool UIWindow::Keyboard_KeyUp(char key)
{
	if(contains)
	{
		return contains->Keyboard_KeyUp(key);
	}
	return true;
}

bool UIWindow::Keyboard_KeyDown(char key)
{
	if(contains)
	{
		return contains->Keyboard_KeyDown(key);
	}
	return true;
}

bool UIWindow::Mouse_ButtonUp(int button)
{
	if(contains)
	{
		return contains->Mouse_ButtonUp(button);
	}
	return true;
}

bool UIWindow::Mouse_ButtonDown(int button)
{
	if(contains)
	{
		return contains->Mouse_ButtonDown(button);
	}
	return true;
}

bool UIWindow::Mouse_Move(POINT p, int mb1, int mb2, int mb3, int mb4)
{
	if(contains)
	{
		return contains->Mouse_Move(p, mb1, mb2, mb3, mb4);
	}
	return true;
}

bool UIWindow::Mouse_Scroll(long scroll)
{
	if(contains)
	{
		return contains->Mouse_Scroll(scroll);
	}
	return true;
}