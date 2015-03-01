#include "UIInputHandler.h"

UIInputHandler::UIInputHandler()
{
	m_ui = NULL;
}

bool UIInputHandler::Keyboard_KeyDown(char key)
{
	if(m_ui->m_active_window)
	{
		m_ui->m_active_window->Keyboard_KeyDown(key);
		return false;
	}
	return true;
}

bool UIInputHandler::Keyboard_KeyUp(char key)
{
	if(m_ui->m_active_window)
	{
		m_ui->m_active_window->Keyboard_KeyUp(key);
		return false;
	}
	return true;
}

bool UIInputHandler::Mouse_ButtonDown(int button)
{
	UserInterface::window_iter i;
	POINT p;
	m_ui->GetMousePos(&p);	
	UIWindow* win;
	for(i = m_ui->windows.begin(); i != m_ui->windows.end(); i++)
	{
		if(!(*i)->Active())
			continue;
		win = (*i)->HitTest(p.x, p.y);
		if(win)
		{
			m_ui->m_active_window = win;
			win->Mouse_ButtonDown(button);
			return false;
		}
		if((*i)->TitlebarHitTest(p.x, p.y))
		{
			m_ui->m_active_window = (*i);
			
			return false;
		}
	}
	if(m_ui->m_active_window)
	{
		m_ui->m_active_window = NULL;
		return true;
	}
	return true;
}

bool UIInputHandler::Mouse_ButtonUp(int button)
{
	UserInterface::window_iter i;
	POINT p;
	m_ui->GetMousePos(&p);
	UIWindow* win;
	for(i = m_ui->windows.begin(); i != m_ui->windows.end(); i++)
	{
		if(!(*i)->Active())
			continue;
		win = (*i)->HitTest(p.x, p.y);
		if(win)
		{
			m_ui->m_active_window = win;
			win->Mouse_ButtonUp(button);
			return false;
		}
		if((*i)->TitlebarHitTest(p.x, p.y))
		{
			m_ui->m_active_window = (*i);
			
			return false;
		}
	}
	if(m_ui->m_active_window)
	{
		m_ui->m_active_window = NULL;
		return false;
	}
	return true;
}

bool UIInputHandler::Mouse_Move(POINT p, int mb1, int mb2, int mb3, int mb4)
{
	POINT p2 = p;
	UIWindow* win;
	if(m_ui->m_active_window)
	{
		win = m_ui->m_active_window->HitTest(p.x, p.y);
		if(win)
		{
			POINT wp = m_ui->m_active_window->GetPos();
			p2.x -= wp.x;
			p2.y -= wp.y;
			m_ui->m_active_window->Mouse_Move(p2, mb1, mb2, mb3, mb4);
			return false;
		}
	}
	return true;
}

bool UIInputHandler::Mouse_Scroll(long scroll)
{
	if(m_ui->m_active_window)
	{
		m_ui->m_active_window->Mouse_Scroll(scroll);
		return false;
	}
	return true;	
}

void UIInputHandler::SetUI(UserInterface* ui)
{
	m_ui = ui;
}
