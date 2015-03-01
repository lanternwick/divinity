#ifndef TOW_INPUTHANDLER_H
#define TOW_INPUTHANDLER_H

#include "..\DivUserInterface\IInputHandler.h"
#include "..\DivUserInterface\DivuserInterface.h"
#include "..\DivUserInterface\UIConsoleWnd.h"

class TOW_InputHandler : public IInputHandler
{
public:
	TOW_InputHandler();
	virtual ~TOW_InputHandler();

	bool Keyboard_KeyUp(char key);
	bool Keyboard_KeyDown(char key);

	bool Mouse_ButtonUp(int button);
	bool Mouse_ButtonDown(int button);
	bool Mouse_Move(POINT p, int mb1, int mb2, int mb3, int m4);
	bool Mouse_Scroll(long scroll);

	void SetTOWMod(TimesOfWar_Mod* towm);

private:
	TimesOfWar_Mod* m_towm;
	bool shift;
};

#endif