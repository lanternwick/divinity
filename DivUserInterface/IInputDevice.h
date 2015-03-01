#ifndef _IINPUTDEVICE_H
#define _IINPUTDEVICE_H

#include "..\Common\Result.h"
#include "..\Common\IThinker.h"
#include "..\Common\IObject.h"
#include "IInputHandler.h"

class IInputDevice : public IThinker, public IObject
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

	virtual RESULT SetInputHandler(IInputHandler* iih)=0;
	virtual char KeyToChar(DWORD keyConst, bool shift)=0;
protected:
	HWND m_hWnd;
};

#endif