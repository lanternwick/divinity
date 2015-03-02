#ifndef UIBASEWINDOW_H
#define UIBASEWINDOW_H

#include "UIWindow.h"
#include "Quad2D.h"
#include <vector>

class UIBaseWindow : public UIWindow
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

	virtual bool Keyboard_KeyUp(IInputDevice* input, char key);
	virtual bool Keyboard_KeyDown(IInputDevice* input, char key);

	virtual bool Mouse_ButtonUp(IInputDevice* input, int button);
	virtual bool Mouse_ButtonDown(IInputDevice* input, int button);

	virtual bool Mouse_Move(IInputDevice* input, POINT p, int mb1, int mb2, int mb3, int m4);
	virtual bool Mouse_Scroll(IInputDevice* input, long scroll);

	unsigned long Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, 
		ITexture* texture, UIWindow* parent);
	RESULT Render();
	RESULT Think();

	int AttachChildWindow(UIWindow* child);
	int Focus();
	int Unfocus();

	virtual float sx();
	virtual float sy();
	virtual float scx();
	virtual float scy();

	virtual void Update();
protected:
	
	Quad2D* m_window;
	ITexture* m_texture;
	VECTOR m_pos;
	SIZE m_size;
	unsigned long m_color;
	bool m_dirty;
	bool m_active;
	bool m_visible;
	std::vector<UIWindow*> m_children;
	bool m_focused;
	UIWindow* m_parent;
	

	
};

#endif