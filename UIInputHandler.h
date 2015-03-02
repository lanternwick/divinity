#ifndef UIINPUTHANDLER_H
#define UIINPUTHANDLER_H

#include "UserInterface.h"
#include "IInputHandler.h"

class IInputDevice;

class UIInputHandler : public IInputHandler
{
public:
	UIInputHandler();
	bool Keyboard_KeyDown(IInputDevice* input, char key);
	bool Keyboard_KeyUp(IInputDevice* input, char key);
	bool Mouse_ButtonDown(IInputDevice* input, int button);
	bool Mouse_ButtonUp(IInputDevice* input, int button);
	bool Mouse_Move(IInputDevice* input, POINT p, int mb1, int mb2, int mb3, int mb4);
	bool Mouse_Scroll(IInputDevice* input, long scroll);

	void SetUI(UserInterface* ui);

protected:
	UserInterface* m_ui;
};

#endif