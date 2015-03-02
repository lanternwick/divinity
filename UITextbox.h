#ifndef UITEXTBOX_H
#define UITEXTBOX_H

#include "UIBaseWindow.h"
#include "Font.h"

class UITextbox : public UIBaseWindow
{
public:
	UITextbox();
	~UITextbox();

	int Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, unsigned long t_color,
		ITexture* texture, Font* fnt, UIWindow* parent);
	
	virtual bool Keyboard_KeyUp(IInputDevice* input, char key);
	virtual bool Keyboard_KeyDown(IInputDevice* input, char key);

	RESULT Render();
	void AttachTo(UIWindow* win);
private:
	std::string m_text;
	Font* m_font;
	unsigned long m_t_color;
	UIWindow* m_attached;

};

#endif
