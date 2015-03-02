#ifndef UITITLEBAR_H
#define UITITLEBAR_H

#include "UIWindow.h"
#include "..\DivGraph\Quad2D.h"


class _API UITitlebar : public UIWindow
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

	virtual void send_message(window_message& msg);
	virtual UIWindow* hit_test(float xp, float yp);
	virtual UIWindow* hit_test(float xp, float yp, float& cxp, float& cyp);

	unsigned long Create(IDevice* device, IGraphObjFactory* factory, int cyp, unsigned long color, moniker& texture, UIWindow* wrap);
	int titlebar_y() const;
	void titlebar_y(int tby);

	RESULT Render();
	RESULT Think();
protected:
	Quad2D* m_bar;
	int m_y;
	moniker m_texture;
	bool m_dirty;
	unsigned long m_color;
	void Update();

};

#endif
