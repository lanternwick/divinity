#include "UserInterface.h"
#include "DIDevice.h"

UserInterface::UserInterface()
{
	m_input = new DIDevice;
	m_font_man = new FontManager;
	m_window_man = new WindowManager;
}

UserInterface::~UserInterface()
{
	if(m_input)
	{
		delete m_input;
	}
	if(m_font_man)
	{
		delete m_font_man;
	}
	if(m_window_man)
	{
		delete m_window_man;
	}
	m_input = NULL;
	m_font_man = NULL;
	m_window_man = NULL;
}

RESULT UserInterface::Render()
{
	WindowManager::window_iter i;
	for(i = m_window_man->Iterator_Begin(); i != m_window_man->Iterator_End(); ++i)
	{
		if(!(*i))
		{
			continue;
		}
		(*i)->Render();
	}
	return OK;
}

RESULT UserInterface::Think()
{
	m_input->Think();
	WindowManager::window_iter i;
	for(i = m_window_man->Iterator_Begin(); i != m_window_man->Iterator_End(); ++i)
	{
		if(!(*i))
		{
			continue;
		}
		(*i)->Think();
	}
	return OK;
}

FontManager& UserInterface::Fonts()
{
	
	return *m_font_man;
}

IInputDevice& UserInterface::Input()
{
	return *m_input;
}

WindowManager& UserInterface::Windows()
{
	return *m_window_man;
}

/*
UIWindow& UserInterface::Window(moniker& window_name)
{
	//temp
	return UIWindow();
}
*/