#ifndef _IUICOMPONENT_H
#define _IUICOMPONENT_H

#include "IObject.h"
#include "IRenderable.h"
#include "IThinker.h"

struct BUTTON_STATE
{
	bool ButtonL;
	bool ButtonR;
	bool ButtonM;
	bool IgnoreL;
	bool IgnoreR;
	bool IgnoreM;
};




class _API IUIComponent : public IObject, IRenderable, IThinker
{
public:
	IUIComponent();
	virtual ~IUIComponent();

	virtual RESULT Render()=0;
	virtual RESULT Think()=0;

	bool HitTest(int x, int y);


protected:
	SIZE m_size;
	POINT m_pos;
	BUTTON_STATE buttons;

};

#endif