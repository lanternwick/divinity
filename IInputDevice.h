#ifndef _IINPUTDEVICE_H
#define _IINPUTDEVICE_H

#include "Result.h"
//#include "IThinker.h"
#include "IInputHandler.h"

class IInputDevice
{
public:
	IInputDevice() {}
	virtual ~IInputDevice() {}

	virtual RESULT CreateDevice(HWND hWnd)=0;

	virtual bool GetKey(unsigned long keyId)=0;
	virtual bool GetMouseButton(unsigned long buttonID)=0;
	virtual RESULT GetMousePos(POINT* pt)=0;
	virtual RESULT GetMouseScroll(long* scroll)=0;

	virtual RESULT Think()=0;

	virtual RESULT SetInputHandler(IInputHandler* iih, bool free=false)=0;
	virtual char KeyToChar(DWORD keyConst, bool shift)=0;
	virtual RESULT CaptureDevices()=0;
	virtual RESULT UncaptureDevices()=0;
protected:
	HWND m_hWnd;
};

#endif