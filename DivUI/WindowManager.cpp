#include "WindowManager.h"

WindowManager::WindowManager()
{
	m_window_list.push_front(NULL);
}

WindowManager::~WindowManager()
{
	while(!m_window_list.empty())
	{
		delete m_window_list.back();
		m_window_list.pop_back();
	}
}

moniker& WindowManager::UICreateWindow(window_def& definition)
{
	
	return definition.window_name;
}

UIWindow& WindowManager::Window(moniker& window_name)
{
	for(std::list<UIWindow*>::iterator i = m_window_list.begin(); i != m_window_list.end(); ++i)
	{
		if(!(*i))
		{
			continue;
		}
		if((*i)->window_name()() == window_name())
		{
			return *(*i);
		}
	}
	throw;
	
}

UIWindow* WindowManager::ActiveWindow()
{
	if(!m_window_list.empty())
	{
		return m_window_list.front();
	}
	return NULL;
}

void WindowManager::ActiveWindow(UIWindow* new_act)
{
	m_window_list.remove(new_act);
	if(!ActiveWindow())
	{
		m_window_list.remove(NULL);
	}
	m_window_list.push_front(new_act);
}

void WindowManager::AttachWindow(UIWindow* window)
{
	m_window_list.push_back(window);
}

WindowManager::window_iter WindowManager::Iterator_Begin()
{
	return m_window_list.begin();
}

WindowManager::window_iter WindowManager::Iterator_End()
{
	return m_window_list.end();
}
