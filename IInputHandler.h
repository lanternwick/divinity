#ifndef IINPUT_HANDLER_H
#define IINPUT_HANDLER_H

#include <windows.h>

class IInputDevice;

class IInputHandler
{
public:
	IInputHandler(){};
	virtual ~IInputHandler(){};

	virtual bool Keyboard_KeyUp(IInputDevice* input, char key){return true;};
	virtual bool Keyboard_KeyDown(IInputDevice* input, char key){return true;};

	virtual bool Mouse_ButtonUp(IInputDevice* input, int button){return true;};
	virtual bool Mouse_ButtonDown(IInputDevice* input, int button){return true;};

	virtual bool Mouse_Move(IInputDevice* input, POINT p, int mb1, int mb2, int mb3, int m4){return true;};
	virtual bool Mouse_Scroll(IInputDevice* input, long scroll){return true;};
private:
	
};

#endif