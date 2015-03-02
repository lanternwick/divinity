#include "UIInputHandler.h"

UIInputHandler::UIInputHandler()
{
	m_ui = NULL;
}

bool UIInputHandler::Keyboard_KeyDown(char key)
{
	if(m_ui->Windows().ActiveWindow())
	{
		m_ui->Windows().ActiveWindow()->Keyboard_KeyDown(key);
		return false;
	}
	return true;
}

bool UIInputHandler::Keyboard_KeyUp(char key)
{
	if(m_ui->Windows().ActiveWindow())
	{
		m_ui->Windows().ActiveWindow()->Keyboard_KeyUp(key);
		return false;
	}
	return true;
}

bool UIInputHandler::Mouse_ButtonDown(int button)
{
	WindowManager::window_iter i;
	POINT p;
	m_ui->Input().GetMousePos(&p);	
	UIWindow* win;
	for(i = m_ui->Windows().Iterator_Begin(); i != m_ui->Windows().Iterator_End(); ++i)
	{
		if(!(*i))
		{
			continue;
		}
		if(!(*i)->active())
			continue;
		win = (*i)->hit_test(p.x, p.y);
		if(win)
		{
			m_ui->Windows().ActiveWindow(win);
			win->Mouse_ButtonDown(button);
			return false;
		}
	}
	if(m_ui->Windows().ActiveWindow())
	{
		m_ui->Windows().ActiveWindow(NULL);
		return true;
	}
	return true;
}

bool UIInputHandler::Mouse_ButtonUp(int button)
{
	WindowManager::window_iter i;
	POINT p;
	m_ui->Input().GetMousePos(&p);
	UIWindow* win;
	for(i = m_ui->Windows().Iterator_Begin(); i != m_ui->Windows().Iterator_End(); ++i)
	{
		if(!(*i))
		{
			continue;
		}
		if(!(*i)->active())
			continue;
		win = (*i)->hit_test(p.x, p.y);
		if(win)
		{
			m_ui->Windows().ActiveWindow(win);
			win->Mouse_ButtonUp(button);
			return false;
		}
	}
	if(m_ui->Windows().ActiveWindow())
	{
		m_ui->Windows().ActiveWindow(NULL);
		return true;
	}
	return true;
}

bool UIInputHandler::Mouse_Move(POINT p, int mb1, int mb2, int mb3, int mb4)
{
	float x, y;
	UIWindow* win;
	if(m_ui->Windows().ActiveWindow())
	{
		win = m_ui->Windows().ActiveWindow()->hit_test(p.x, p.y, x, y);
		if(win)
		{
			POINT p2 = {x, y};
			m_ui->Windows().ActiveWindow()->Mouse_Move(p2, mb1, mb2, mb3, mb4);
			return false;
		}
	}
	return true;
}

bool UIInputHandler::Mouse_Scroll(long scroll)
{
	if(m_ui->Windows().ActiveWindow())
	{
		m_ui->Windows().ActiveWindow()->Mouse_Scroll(scroll);
		return false;
	}
	return true;	
}

void UIInputHandler::SetUI(UserInterface* ui)
{
	m_ui = ui;
}
