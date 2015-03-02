#ifndef _DIDEVICE_H
#define _DIDEVICE_H

#define DIRECTINPUT_VERSION 0x0800

#include "IInputDevice.h"
#include <dinput.h>
#include <vector>
#include "keyconstants.h"

class IInputHandler;

class DIDevice : public IInputDevice
{
public:
	DIDevice();
	DIDevice(HWND hWnd);
	virtual ~DIDevice();
	
	virtual RESULT CreateDevice(HWND hWnd);

	virtual bool GetKey(unsigned long keyId);
	virtual bool GetMouseButton(unsigned long buttonID);
	virtual RESULT GetMousePos(POINT* pt);
	virtual RESULT GetMouseScroll(long* scroll);

	virtual RESULT Think();

	virtual RESULT SetInputHandler(IInputHandler* iih);
	virtual char KeyToChar(DWORD keyConst, bool shift);
	virtual RESULT CaptureDevices();
	virtual RESULT UncaptureDevices();
private:
	RESULT ProcessBuffers(char* kb_buffer, DIMOUSESTATE* mouse_data);
	
	LPDIRECTINPUT8 m_device;
	LPDIRECTINPUTDEVICE8 m_keyboard;
	LPDIRECTINPUTDEVICE8 m_mouse;
	bool keys[256];
	bool buttons[4];
	POINT mousePos;
	long mouseZ;
	std::vector<IInputHandler*> m_inputHandlers;
	bool m_released;
	
};

#endif

	