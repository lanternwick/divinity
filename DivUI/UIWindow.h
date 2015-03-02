#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "..\Common\define_exports.h"
#include "..\Common\custmoniker.h"
#include "..\DivGraph\IDevice.h"
#include "..\DivGraph\IGraphObjFactory.h"
#include "..\Common\IRenderable.h"
#include "..\Common\IThinker.h"
#include "IInputHandler.h"


struct window_def
{
	// window name, to refer to window in list
	// set to "unreferenced" if you will not need to access this window
	moniker window_name;
	// positioning and sizing
	int x;
	int y;
	int cx;
	int cy;
	// window color
	unsigned long color;
	// window texture
	// set to "none" if no texture is to be used
	moniker window_texture;
};

struct window_message
{
	unsigned long message;
	std::string string_ret;
	unsigned long ulong_ret[2];
	std::string string_in;
	unsigned long ulong_in[2];
		
};

class _API UIWindow : public IRenderable, public IThinker, public IInputHandler
{
public:
	UIWindow();
	virtual ~UIWindow();

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
	virtual bool Keyboard_KeyUp(char key);
	virtual bool Keyboard_KeyDown(char key);

	virtual bool Mouse_ButtonUp(int button);
	virtual bool Mouse_ButtonDown(int button);

	virtual bool Mouse_Move(POINT p, int mb1, int mb2, int mb3, int m4);
	virtual bool Mouse_Scroll(long scroll);

	virtual const moniker& window_name() const;
protected:
	moniker m_window_name;
	UIWindow* contains;
	IDevice* m_device;
	IGraphObjFactory* m_factory;
	
};

#endif
