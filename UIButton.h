#ifndef UIBUTTON_H
#define UIBUTOON_H

#include "UIBaseWindow.h"
#include "console_functor.h"
#include "Font.h"

class client;

class UIButtonFunctor : public console_functor
{

protected:
	client* m_client;
};

class UIButton : public UIBaseWindow
{
public:
	UIButton();
	~UIButton();

	int Create(IDevice* device, float xp, float yp, float cxp, float cyp, ITexture* texture, unsigned long color, 
		unsigned long color_d, unsigned long t_color, Font* fnt, const std::string& text, const std::string& command, 
		UIWindow* parent);

	virtual bool Mouse_ButtonUp(IInputDevice* input, int button);
	virtual bool Mouse_ButtonDown(IInputDevice* input, int button);

	RESULT Render();
	RESULT Think();

	void Update();
private:
	std::string m_button_cmd;
	std::string m_button_text;
	Font* m_font;
	bool m_down_on_me;
	unsigned long m_color_d;
	unsigned long m_t_color;
	IInputDevice* m_input;

};

#endif
