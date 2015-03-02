#ifndef UISCROLLBAR_H
#define UISCROLLBAR_H

#include "UIBaseWindow.h"

#define SCROLLUP 256
#define SCROLLDN 257
#define SBVALCNG 258

class UIScrollbar : public UIBaseWindow
{
public:
	UIScrollbar();
	~UIScrollbar();

	int Create(IDevice* device, float xp, float yp, float cxp, float cyp, ITexture* text, ITexture* arrow, ITexture* thumb,
		unsigned long color, int min, int max, UIWindow* parent);

	virtual bool Mouse_ButtonUp(IInputDevice* input, int button);
	virtual bool Mouse_ButtonDown(IInputDevice* input, int button);

	RESULT Think();
	RESULT Render();

	virtual void Update();

	int value();
	void value(int n);

	void AttachTo(UIWindow* win);
protected:
	Quad2D* m_up;
	Quad2D* m_dn;
	Quad2D* m_thumb;
	ITexture* m_arrow_tex;
	ITexture* m_thumb_tex;
	int m_min;
	int m_max;
	int m_val;
	UIWindow* m_attached;

};

#endif
