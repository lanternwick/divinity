#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "..\Common\define_exports.h"

#include "FontManager.h"
#include "..\Common\custmoniker.h"
#include "UIWindow.h"
#include "IInputDevice.h"
#include "WindowManager.h"


#include <list>

class _API UserInterface
{
public:
	UserInterface();
	~UserInterface();

	virtual RESULT Render();
	virtual RESULT Think();

	FontManager& Fonts();
	IInputDevice& Input();	
	WindowManager& Windows();

private:
	FontManager* m_font_man;
	IInputDevice* m_input;
	WindowManager* m_window_man;
};

#endif

