#ifndef _UIWINDOW_H
#define _UIWINDOW_H


#include "..\Common\Result.h"

#include "..\Common\IUIComponent.h"
#include "..\DivGraph\DivGraph.h"
#include "..\DivCore\DivCore.h"
#include "..\DivGraph\Quad2D.h"
#include "IInputhandler.h"
#include "FontManager.h"

class _API UIWindow : public IUIComponent
{
public:
	
	UIWindow(char* id);
	virtual ~UIWindow();

	virtual RESULT Create(IDevice* device, IGraphObjFactory* factory, float x, float y, float cx, float cy, 
						  unsigned long color, unsigned long caption_color, bool has_titlebar=false, bool has_border=false,
						  float titlebar_height=1.3f, unsigned long t_color=0, float border_width=1.0f,
						  unsigned long border_color=0, float title_bar_overlap=0.0f, float border_overlap=0.0f,
						  UIWindow* parent=0);
	virtual RESULT Render();
	virtual RESULT Think();
	
	void SetFontManager(FontManager* fontMan);
	void SetTextures(ITexture* mainWnd=NULL, ITexture* titlebar=NULL, ITexture* edgeV=NULL, ITexture* edgeH=NULL, 
					 ITexture* corner=NULL);
	void SetFont(moniker& font);
	void SetCaption(std::string& caption);
	std::string GetCaption();
	UIWindow* HitTest(int x, int y);
	bool TitlebarHitTest(int x, int y);
	int BorderHitTest(int x, int y);
	void SetAttachPoint(POINT p);
	POINT GetAttachPoint();
	void Move(int x, int y);
	void Resize(int x, int y);
	void AddChildWindow(UIWindow* wnd);
	void SetParent(UIWindow* wnd);
	
protected:
	UIWindow();
	ITexture* m_windowTexture;
	ITexture* m_edgeVTexture;
	ITexture* m_edgeHTexture;
	ITexture* m_edgeCornerTexture;
	ITexture* m_titlebarTexture;
	IDevice* m_device;
	IGraphObjFactory* m_factory;
	
	Quad2D* window;
	Quad2D* titleBar;
	Quad2D* border[8];
	TLTVERTEX* m_verts;
	unsigned long m_color;
	unsigned long m_t_color;
	unsigned long m_border_color;
	float m_titlebar_height;
	float m_titlebar_overlap;
	float m_border_overlap;
	int m_borderWidth;
	void m_SyncVerts();
	FontManager* m_fontMan;
	bool m_hasBorder;
	bool m_hasTitlebar;
	std::string m_caption;
	moniker m_font;
	unsigned long m_caption_color;
	POINT m_attach_point;
	std::list<_API UIWindow*> children;
	UIWindow* m_parent;
	IInputHandler* input_handler;

};

#endif
	