#ifndef UIBASEWINDOW_H
#define UIBASEWINDOW_H

#include "UIWindow.h"
#include "..\DivGraph\Quad2D.h"
#include "..\Common\define_exports.h"

class _API UIBaseWindow : public UIWindow
{
public:
	UIBaseWindow();
	virtual ~UIBaseWindow();

	//positioning functions
	virtual float x();
	virtual float y();
	virtual float cx();
	virtual float cy();
	
	virtual void x(float xp);
	virtual void y(float yp);
	virtual void cx(float cxp);
	virtual void cy(float cyp);

	// messaging functions
	virtual void send_message(window_message& msg);
	virtual UIWindow* hit_test(float xp, float yp);
	virtual UIWindow* hit_test(float xp, float yp, float& cxp, float& cyp);
	virtual bool active();
	bool visible();
	void visible(bool v);

	unsigned long Create(IDevice* device, IGraphObjFactory* factory, float xp, float yp, float cxp, float cyp, unsigned long color, moniker& texture, UIWindow* parent);
	RESULT Render();
	RESULT Think();

protected:
	
	Quad2D* m_window;
	moniker m_texture;
	VECTOR m_pos;
	SIZE m_size;
	unsigned long m_color;
	bool m_dirty;
	bool m_active;
	bool m_visible;
	void Update();
};

#endif