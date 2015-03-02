#ifndef UITITLEBAR_H
#define UITITLEBAR_H

#include "UIWindow.h"
#include "Quad2D.h"
#include "Font.h"


class UITitlebar : public UIWindow
{
public:
	UITitlebar();
	virtual ~UITitlebar();
	
	//positioning functions
	virtual float x();
	virtual float y();
	virtual float cx();
	virtual float cy();

	virtual void x(float xp);
	virtual void y(float yp);
	virtual void cx(float cxp);
	virtual void cy(float cyp);

	bool Mouse_ButtonUp(IInputDevice* input, int button);
	bool Mouse_ButtonDown(IInputDevice* input, int button);

	virtual void send_message(window_message& msg);
	virtual UIWindow* hit_test(float xp, float yp);
	virtual UIWindow* hit_test(float xp, float yp, float& cxp, float& cyp);

	unsigned long Create(IDevice* device, int cyp, unsigned long color, unsigned long inactive_color, ITexture* texture, Font* fnt, UIWindow* wrap);
	int titlebar_y() const;
	void titlebar_y(int tby);

	RESULT Render();
	RESULT Think();

	virtual bool AttachToMouse(int offsetx, int offsety);
	virtual bool DetachFromMouse();
	virtual int GetMouseOffset(int& ffsetx, int& offsety);
	virtual bool IsAttachedToMouse();

	virtual int Focus();
	virtual int Unfocus();

	virtual void Update();
protected:
	Quad2D* m_bar;
	int m_y;
	ITexture* m_texture;
	bool m_dirty;
	unsigned long m_color;
	unsigned long m_icolor;
	int m_moffsetx;
	int m_moffsety;
	bool m_attachedtomouse;
	bool m_focused;
	Font* m_font;
	

};

#endif
