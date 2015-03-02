#ifndef UIINPUTHANDLER_H
#define UIINPUTHANDLER_H

#include "UserInterface.h"
#include "IInputHandler.h"

class UIInputHandler : public IInputHandler
{
public:
	UIInputHandler();
	bool Keyboard_KeyDown(char key);
	bool Keyboard_KeyUp(char key);
	bool Mouse_ButtonDown(int button);
	bool Mouse_ButtonUp(int button);
	bool Mouse_Move(POINT p, int mb1, int mb2, int mb3, int mb4);
	bool Mouse_Scroll(long scroll);

	void SetUI(UserInterface* ui);

protected:
	UserInterface* m_ui;
};

#endif