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

bool UIWindow::Keyboard_KeyUp(IInputDevice* input, char key)
{
	if(contains)
	{
		return contains->Keyboard_KeyUp(input, key);
	}
	return true;
}

bool UIWindow::Keyboard_KeyDown(IInputDevice* input, char key)
{
	if(contains)
	{
		return contains->Keyboard_KeyDown(input, key);
	}
	return true;
}

bool UIWindow::Mouse_ButtonUp(IInputDevice* input, int button)
{
	if(contains)
	{
		return contains->Mouse_ButtonUp(input, button);
	}
	return true;
}

bool UIWindow::Mouse_ButtonDown(IInputDevice* input, int button)
{
	if(contains)
	{
		return contains->Mouse_ButtonDown(input, button);
	}
	return true;
}

bool UIWindow::Mouse_Move(IInputDevice* input, POINT p, int mb1, int mb2, int mb3, int mb4)
{
	if(contains)
	{
		return contains->Mouse_Move(input, p, mb1, mb2, mb3, mb4);
	}
	return true;
}

bool UIWindow::Mouse_Scroll(IInputDevice* input, long scroll)
{
	if(contains)
	{
		return contains->Mouse_Scroll(input, scroll);
	}
	return true;
}

bool UIWindow::AttachToMouse(int offsetx, int offsety)
{
	if(contains)
	{
		return contains->AttachToMouse(offsetx, offsety);
	}
	return true;
}

bool UIWindow::DetachFromMouse()
{
	if(contains)
	{
		return contains->DetachFromMouse();
	}
	return true;
}

int UIWindow::GetMouseOffset(int& offsetx, int& offsety)
{
	if(contains)
	{
		return contains->GetMouseOffset(offsetx, offsety);
	}
	return 0;
}

bool UIWindow::IsAttachedToMouse()
{
	if(contains)
	{
		return contains->IsAttachedToMouse();
	}
	return false;
}

int UIWindow::AttachChildWindow(UIWindow* child)
{
	if(contains)
	{
		return contains->AttachChildWindow(child);
	}
	return 0;
}

int UIWindow::Focus()
{
	if(contains)
	{
		return contains->Focus();
	}
	return 0;
}

int UIWindow::Unfocus()
{
	if(contains)
	{
		return contains->Unfocus();
	}
	return 0;
}

float UIWindow::sx()
{
	if(contains)
	{
		return contains->sx();
	}
	throw;
}

float UIWindow::sy()
{
	if(contains)
	{
		return contains->sy();
	}
	throw;
}

float UIWindow::scx()
{
	if(contains)
	{
		return contains->scx();
	}
	throw;
}

float UIWindow::scy()
{
	if(contains)
	{
		return contains->scy();
	}
	throw;
}