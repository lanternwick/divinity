#include "UIWindow.h"

UIWindow::UIWindow()
{
	Core::Function_Enter("UIWindow::Constructor()");
	
	m_verts = NULL;
	m_device = NULL;
	window = NULL;
	m_fontMan = NULL;
	titleBar = NULL;
	m_parent = NULL;
	m_windowTexture = 
		m_titlebarTexture =
		m_edgeVTexture = 
		m_edgeHTexture = 
		m_edgeCornerTexture = NULL;
	for(int i = 0; i < 8; ++i)
	{
		border[i] = NULL;	
	}
	Core::Function_Exit();
}

UIWindow::UIWindow(char* id)
{
	Core::Function_Enter("UIWindow::Constructor()");
	
	m_verts = NULL;
	m_device = NULL;
	window = NULL;
	m_fontMan = NULL;
	m_id() = id;
	titleBar = NULL;
	m_parent = NULL;
	m_windowTexture = 
		m_titlebarTexture =
		m_edgeVTexture = 
		m_edgeHTexture = 
		m_edgeCornerTexture = NULL;
	for(int i = 0; i < 8; ++i)
	{
		border[i] = NULL;	
	}
	Core::Function_Exit();
}

UIWindow::~UIWindow()
{
	Core::Function_Enter("UIWindow::Destructor()");
	if(window) delete window;
	window = NULL;
	if(m_verts) delete m_verts;
	m_verts = NULL;
	if(titleBar)
		delete titleBar;
		
	for(int i = 0; i < 8; ++i)
	{
		if(border[i])
			delete border[i];	
		border[i] = NULL;
	}
	
	
	titleBar = NULL;
	m_windowTexture = 
		m_titlebarTexture =
		m_edgeVTexture = 
		m_edgeHTexture = 
		m_edgeCornerTexture = NULL;
	Core::Function_Exit();
}

RESULT UIWindow::Create(IDevice* device, IGraphObjFactory* factory, float x, float y, float cx, float cy, unsigned long color,
						unsigned long caption_color, bool has_titlebar, bool has_border, float titlebar_height, 
						unsigned long t_color, float border_width, unsigned long border_color,
						float titlebar_overlap, float border_overlap, UIWindow* parent)
{
	Core::Function_Enter("UIWindow::Create()");
	
	m_verts = new TLTVERTEX[4];

	m_device = device;
	m_factory = factory;
	
	POINT p;
	if(!parent)
	{
		p.x = x*device->width();
		p.y = y*device->height();
	}
	else
	{	
		p.x = x*parent->Size().cx;
		p.y = y*parent->Size().cy;
	}
	SetPos(p);
	SIZE s;
	if(!parent)
	{
		s.cx = cx*device->width();
		s.cy = cy*device->height();
	}
	else
	{
		s.cx = cx*parent->Size().cx;
		s.cy = cy*parent->Size().cy;
	}
	SetSize(s);

	m_hasBorder = has_border;
	m_hasTitlebar = has_titlebar;
	m_titlebar_height = titlebar_height;
	m_titlebar_overlap = titlebar_overlap;
	m_border_overlap = border_overlap;
	m_borderWidth = border_width;
	

	m_border_color = border_color;
	m_t_color = t_color;
	m_color = color;
	m_caption_color = caption_color;

	if(parent)
		parent->AddChildWindow(this);

	m_SyncVerts();
	
	Core::Function_Exit();
	return OK;
}

RESULT UIWindow::Render()
{
	Core::Function_Enter("UIWindow::Render()");
	RECT re = {m_pos.x+3, m_pos.y - m_titlebar_height+m_titlebar_overlap + 3, m_pos.x + m_size.cx-3, m_pos.y - 3 };
	if(m_windowTexture)
		m_windowTexture->Set(0);
	else
		ITexture::MakeNull(m_device, 0);
	m_device->SetTextureState(0, TS_COLOROP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_COLORARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_COLORARG2, TA_DIFFUSE);
	m_device->SetTextureState(0, TS_ALPHAOP, TOP_MODULATE);
	m_device->SetTextureState(0, TS_ALPHAARG1, TA_TEXTURE);
	m_device->SetTextureState(0, TS_ALPHAARG2, TA_DIFFUSE);
	m_device->SetTextureState(0, TS_MAGFILTER, 0);
	m_device->SetTextureState(0, TS_MINFILTER, 0);

	RESULT r = window->Render();
	if(m_hasTitlebar)
	{
		if(m_titlebarTexture)
			m_titlebarTexture->Set(0);
		else
			ITexture::MakeNull(m_device, 0);
		r = titleBar->Render();
	}
	if(m_hasBorder)
	{
		if(m_edgeVTexture)
			m_edgeVTexture->Set(0);
		else
			ITexture::MakeNull(m_device, 0);
		border[0]->Render();
		border[2]->Render();
		if(m_edgeHTexture)
			m_edgeHTexture->Set(0);
		else
			ITexture::MakeNull(m_device, 0);
		border[1]->Render();
		border[3]->Render();

		if(m_edgeCornerTexture)
			m_edgeCornerTexture->Set(0);
		else
			ITexture::MakeNull(m_device, 0);
		border[4]->Render();
		border[5]->Render();
		border[6]->Render();
		border[7]->Render();
	}
	if(m_caption.size() && m_hasTitlebar)
	{
		m_fontMan->DrawText(m_font, m_caption.c_str(), re, NULL, 1.0f, m_caption_color);
	}
	std::list<_API UIWindow*>::iterator i;
	for(i = children.begin(); i != children.end(); i++)
		(*i)->Render();
	RenderExtra();
	Core::Function_Exit();
	return r;
}

RESULT UIWindow::Think()
{
	Core::Function_Enter("UIWindow::Think()");
	m_SyncVerts();
	Core::Function_Exit();
	return OK;
}

void UIWindow::m_SyncVerts()
{
	m_verts[0] = TLTVertex(m_pos.x, m_pos.y, 0.0f, 1.0f, m_color, 0.0f, 0.0f);
	m_verts[1] = TLTVertex(m_pos.x+m_size.cx, m_pos.y, 0.0f, 1.0f, m_color, 1.0f, 0.0f);
	m_verts[2] = TLTVertex(m_pos.x, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 0.0f, 1.0f);
	m_verts[3] = TLTVertex(m_pos.x+m_size.cx, m_pos.y+m_size.cy, 0.0f, 1.0f, m_color, 1.0f, 1.0f);

	if(window)
		delete window;
	window = new Quad2D;
	window->Create(m_device, m_factory, m_verts);
	long x = m_pos.x, y = m_pos.y;
	long cx = m_size.cx, cy = m_size.cy;
	float t;
	if(m_hasTitlebar)
	{
		if(titleBar)
			delete titleBar;
		m_verts[0] = TLTVertex(x, y-m_titlebar_height+m_titlebar_overlap, 0.1f, 1.0f, m_t_color, 0.0f, 0.0f);
		if(m_titlebarTexture)
			t = float(cx) / float(m_titlebar_height);
		m_verts[1] = TLTVertex(x+cx, y-m_titlebar_height+m_titlebar_overlap, 0.1f, 1.0f, m_t_color, t, 0.0f);
		m_verts[2] = TLTVertex(x, y+m_titlebar_overlap, 0.1f, 1.0f, m_t_color, 0.0f, 1.0f);
		m_verts[3] = TLTVertex(x+cx, y+m_titlebar_overlap, 0.1f, 1.0f, m_t_color, t, 1.0f);
		titleBar = new Quad2D;
		titleBar->Create(m_device, m_factory, m_verts);
	}
	long border_y;
	long border_width = m_borderWidth;
	if(m_hasBorder)
	{
		for(int i = 0; i < 8; ++i)
		{
			if(border[i])
				delete border[i];
			border[i] = new Quad2D;
		}
		
		// start border 1
		border_y = m_hasTitlebar ? y - m_titlebar_height : y;
		border_y+=m_titlebar_overlap;
		m_verts[0] = TLTVertex(x-border_width, border_y, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[1] = TLTVertex(x, border_y, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);
		m_verts[2] = TLTVertex(x-border_width, y+cy, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[3] = TLTVertex(x, y+cy, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);

		border[0]->Create(m_device, m_factory, m_verts);

		// start border 2
		border_y = m_hasTitlebar ? y - m_titlebar_height : y;
		border_y += m_titlebar_overlap;
		m_verts[0] = TLTVertex(x, border_y-border_width, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[1] = TLTVertex(x+cx, border_y-border_width, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);
		m_verts[2] = TLTVertex(x, border_y, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[3] = TLTVertex(x+cx, border_y, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);

		border[1]->Create(m_device, m_factory, m_verts);

		// start border 3
		border_y = m_hasTitlebar ? y - m_titlebar_height : y;
		border_y+=m_titlebar_overlap;
		m_verts[0] = TLTVertex(x+cx+border_width, border_y, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[1] = TLTVertex(x+cx, border_y, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);
		m_verts[2] = TLTVertex(x+cx+border_width, y+cy, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[3] = TLTVertex(x+cx, y+cy, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);

		border[2]->Create(m_device, m_factory, m_verts);

		// start border 4
		border_y = y;
		m_verts[0] = TLTVertex(x, border_y+cy+border_width, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[1] = TLTVertex(x+cx, border_y+cy+border_width, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);
		m_verts[2] = TLTVertex(x, border_y+cy, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[3] = TLTVertex(x+cx, border_y+cy, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);

		border[3]->Create(m_device, m_factory, m_verts);

		// start border 5
		border_y = m_hasTitlebar ? y - m_titlebar_height : y;
		border_y += m_titlebar_overlap;
		m_verts[0] = TLTVertex(x-border_width, border_y-border_width, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[1] = TLTVertex(x, border_y-border_width, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);
		m_verts[2] = TLTVertex(x-border_width, border_y, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[3] = TLTVertex(x, border_y, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);

		border[4]->Create(m_device, m_factory, m_verts);

		// start border 6
		border_y = m_hasTitlebar ? y - m_titlebar_height : y;
		border_y += m_titlebar_overlap;
		m_verts[0] = TLTVertex(x+cx+border_width, border_y-border_width, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[1] = TLTVertex(x+cx, border_y-border_width, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);
		m_verts[2] = TLTVertex(x+cx+border_width, border_y, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[3] = TLTVertex(x+cx, border_y, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);

		border[5]->Create(m_device, m_factory, m_verts);

		// start border 7
		border_y = y+cy;
		m_verts[0] = TLTVertex(x-border_width, border_y, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[1] = TLTVertex(x, border_y, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);
		m_verts[2] = TLTVertex(x-border_width, border_y+border_width, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[3] = TLTVertex(x, border_y+border_width, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);

		border[6]->Create(m_device, m_factory, m_verts);

		// start border 8
		border_y = y+cy;
		m_verts[0] = TLTVertex(x+cx+border_width, border_y, 0.1f, 1.0f, m_border_color, 0.0f, 1.0f);
		m_verts[1] = TLTVertex(x+cx, border_y, 0.1f, 1.0f, m_border_color, 1.0f, 1.0f);
		m_verts[2] = TLTVertex(x+cx+border_width, border_y+border_width, 0.1f, 1.0f, m_border_color, 0.0f, 0.0f);
		m_verts[3] = TLTVertex(x+cx, border_y+border_width, 0.1f, 1.0f, m_border_color, 1.0f, 0.0f);

		border[7]->Create(m_device, m_factory, m_verts);
	}
}

void UIWindow::SetFontManager(FontManager* fontMan)
{
	m_fontMan = fontMan;
}

void UIWindow::SetTextures(ITexture* mainWnd, ITexture* titlebar, ITexture* edgeV, 
						   ITexture* edgeH, ITexture* corner)
{
	if(mainWnd)
		m_windowTexture = mainWnd;
	if(titlebar)
		m_titlebarTexture = titlebar;
	if(edgeV)
		m_edgeVTexture = edgeV;
	if(edgeH)
		m_edgeHTexture = edgeH;
	if(corner)
		m_edgeCornerTexture = corner;
}

void UIWindow::SetFont(moniker& font)
{
	m_font() = font();
}

void UIWindow::SetCaption(std::string& caption)
{
	m_caption = caption;
}

std::string UIWindow::GetCaption()
{
	return m_caption;
}

UIWindow* UIWindow::HitTest(int x, int y)
{
	Core::Function_Enter("IUIComponent::HitTest()");
	RECT r;
	r.left = m_pos.x;
	r.right = m_pos.x + m_size.cx;
	r.top = m_pos.y;
	r.bottom = m_pos.y + m_size.cy;
	POINT p = {x, y};
	bool b = PtInRect(&r, p)!=0;
	UIWindow* wind;
	if(b)
	{
		std::list<_API UIWindow*>::iterator i;
		for(i = children.begin(); i != children.end(); i++)
		{
			wind = (*i)->HitTest(x, y);
			if(wind)
			{
				Core::Function_Exit();
				return wind;
			}
		}
		Core::Function_Exit();
		return this;
	}
	else
	{
		Core::Function_Exit();
		return NULL;
	}
}

bool UIWindow::TitlebarHitTest(int x, int y)
{
	if(!m_hasTitlebar)
		return false;
	RECT r = {m_pos.x, m_pos.y - m_titlebar_height+m_titlebar_overlap, m_pos.x + m_size.cx, m_pos.y};
	POINT p = { x, y };
	return PtInRect(&r, p) != 0;
}

int UIWindow::BorderHitTest(int x, int y)
{
	long border_width = m_borderWidth;

	float titlebar_height = m_hasTitlebar ? m_titlebar_height : 0.0f;
	RECT r = {m_pos.x-border_width-5, m_pos.y - m_titlebar_height+m_titlebar_overlap, m_pos.x+5, m_pos.y+m_size.cy};
	POINT p = { x, y };
	if(PtInRect(&r, p))
	{
		return 1;
	}
	r.left += m_size.cx;
	r.right += m_size.cx;
	if(PtInRect(&r, p))
	{
		return 3;
	}
	r.left = m_pos.x - border_width-5;
	r.right = m_pos.x + border_width + m_size.cx+5;
	r.top = m_pos.y - titlebar_height-border_width;
	r.bottom = r.top +border_width;
	if(PtInRect(&r, p))
	{
		return 2;
	}
	r.top += m_size.cy + titlebar_height;
	r.bottom += m_size.cy + titlebar_height;
	if(PtInRect(&r, p))
	{
		return 4;
	}
	r.top = m_pos.y - border_width - titlebar_height+m_titlebar_overlap;
	r.bottom = r.top + border_width;
	r.left = m_pos.x - border_width+5;
	r.right = r.left + border_width-5;
	if(PtInRect(&r, p))
	{
		return 5;
	}
	r.left += m_size.cx;
	r.right += m_size.cx;
	if(PtInRect(&r, p))
	{
		return 6;
	}
	r.top += m_size.cy;
	r.bottom += m_size.cy;
	if(PtInRect(&r, p))
	{
		return 7;
	}
	r.left -= m_size.cx;
	r.right -= m_size.cx;
	if(PtInRect(&r, p))
	{
		return 8;
	}
	return 0;
}

void UIWindow::SetAttachPoint(POINT p)
{
	m_attach_point = p;
}

POINT UIWindow::GetAttachPoint()
{
	return m_attach_point;
}

void UIWindow::Move(int x, int y)
{
	POINT pPos = {0,0};
	if(m_parent)
	{
		pPos = m_parent->GetPos();
		pPos.x += m_actual_pos.x;
		pPos.y += m_actual_pos.y;

	}
	else
	{
		pPos.x = x;
		pPos.y = y;
	}
	POINT p = {pPos.x, pPos.y};
	SetPos(p);
	m_SyncVerts();
	std::list<_API UIWindow*>::iterator i;
	for(i = children.begin(); i != children.end(); i++)
		(*i)->Move(x, y);


}
	
void UIWindow::Resize(int x, int y)
{
	SIZE s = {x, y};
	SetSize(s);
	m_SyncVerts();
}

void UIWindow::AddChildWindow(UIWindow* wnd)
{
	wnd->SetParent(this);
	children.push_back(wnd);
}

void UIWindow::SetParent(UIWindow* wnd)
{
	m_parent = wnd;
	m_actual_pos = m_pos;
}