#ifndef IINPUT_HANDLER_H
#define IINPUT_HANDLER_H

#include <windows.h>
#include "..\DivCore\DivCore.h"


class _API IInputHandler
{
public:
	IInputHandler(){};
	virtual ~IInputHandler(){};

	virtual bool Keyboard_KeyUp(char key){return true;};
	virtual bool Keyboard_KeyDown(char key){return true;};

	virtual bool Mouse_ButtonUp(int button){return true;};
	virtual bool Mouse_ButtonDown(int button){return true;};

	virtual bool Mouse_Move(POINT p, int mb1, int mb2, int mb3, int m4){return true;};
	virtual bool Mouse_Scroll(long scroll){return true;};
};

#endif