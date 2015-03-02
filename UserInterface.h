#ifndef USERINTERFACE_H
#define USERINTERFACE_H


#include "UIWindow.h"
#include "FontManager.h"
#include "custmoniker.h"

#include "IInputDevice.h"
#include "WindowManager.h"


#include <list>

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	virtual RESULT Render();
	virtual RESULT Think();

	FontManager& Fonts();
	IInputDevice& Input();	
	WindowManager& Windows();

	int Release();
private:
	FontManager* m_font_man;
	IInputDevice* m_input;
	WindowManager* m_window_man;
};

#endif

