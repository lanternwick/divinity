#ifndef UICHECKBOX_H
#define UICHECKBOX_H

#include "UIBaseWindow.h"
#include "Font.h"

class UICheckbox : public UIBaseWindow
{
public:
	UICheckbox();
	~UICheckbox();

	int	Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, 
		unsigned long t_color, Font* fnt, ITexture* checked, ITexture* unchecked, 
		const std::string& text, UIWindow* parent);

	bool Mouse_ButtonUp(IInputDevice* input, int button);

	RESULT Render();
	void Update();
private:
	std::string m_text;
	bool m_val;
	ITexture* m_checked_tex;
	ITexture* m_unchecked_tex;
	Font* m_font;
	unsigned long m_t_color;
};

#endif
