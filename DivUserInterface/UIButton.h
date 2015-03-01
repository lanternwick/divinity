#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "UIWindow.h"

class _API UIButton : public UIWindow
{
public:
	UIButton(char* id);
	virtual RESULT Create(IDevice* device, IGraphObjFactory* factory, float x, float y, float cx, float cy, unsigned long color, unsigned long caption_color, bool border=false, unsigned long border_color=0, float border_width=0.0f, UIWindow* parent=NULL);
	virtual RESULT Render();
	virtual RESULT Think();
	virtual bool Mouse_ButtonUp(int button);
protected:
	UIButton();
};

#endif