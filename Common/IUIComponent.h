#ifndef _IUICOMPONENT_H
#define _IUICOMPONENT_H

#include "IEntity.h"

#include "..\Common\IMediaObject.h"
#include "..\DivCore\DivCore.h"
#include <string>

#include "..\DivUserInterface\IInputHandler.h"

struct BUTTON_STATE
{
	bool ButtonL;
	bool ButtonR;
	bool ButtonM;
	bool IgnoreL;
	bool IgnoreR;
	bool IgnoreM;
};




class _API IUIComponent : public IMediaObject, public IInputHandler, public IThinker, public IRenderable
{
public:
	
	IUIComponent(char* id);
	virtual ~IUIComponent();

	virtual RESULT Render()=0;
	virtual RESULT RenderExtra(){return OK;};
	virtual RESULT Think()=0;

	
	POINT GetPos();
	void SetPos(POINT p);
	SIZE Size();
	void SetSize(SIZE s);
	virtual void AddKeyToBuffer(char c);
	std::string GetBuffer();
	void ClearBuffer();
	void SetInputHandler(IInputHandler* iih);
	void Activate();
	void Deactivate();
	bool Active();
	

protected:
	IUIComponent();
	SIZE m_size;
	POINT m_pos;
	POINT m_actual_pos;
	BUTTON_STATE buttons;
	std::string m_input_buffer;
	IInputHandler* m_input_handler;
	bool active;

};

#endif