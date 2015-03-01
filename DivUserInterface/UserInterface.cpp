#include "UserInterface.h"

UserInterface* UserInterface::_instance = NULL;

UserInterface::UserInterface()
{
	input = new DIDevice;
	cursor = NULL;
	m_mouse_down1 = false;
	m_mouse_down2 = false;
	m_attached_window = NULL;
	m_active_window = NULL;
}

UserInterface::~UserInterface()
{
	if(input)
	{
		delete input;
		input = NULL;
	}
	if(m_fontMan)
	{
		delete m_fontMan;
		m_fontMan = NULL;
	}
	if(cursor)
	{
		delete cursor;
		cursor = NULL;
	}
}

UserInterface* UserInterface::Instance()
{
	if(!_instance)
		_instance = new UserInterface;
	return _instance;
}

RESULT UserInterface::Initialize(HWND hWnd, IDevice* device, IGraphObjFactory* factory)
{
	
	input->CreateDevice(hWnd);
	m_device = device;
	m_factory = factory;
	m_fontMan = new FontManager;
	m_fontMan->SetDevice(m_device);
	m_fontMan->SetFactory(m_factory);
	cursor = new UICursor;
	cursor->Create(m_device, m_factory, m_fontMan);
	cursor->Think();
	return OK;
}

RESULT UserInterface::LoadFont(char* filename, moniker& fontname)
{
	RESULT r = m_fontMan->LoadFont(filename, fontname);
	return r;
}

RESULT UserInterface::RemoveFont(moniker& fontname)
{
	RESULT r = m_fontMan->RemoveFont(fontname);
	return r;
}

RESULT UserInterface::DrawText(const moniker& font, const char* text, RECT text_rect, char* attributes, float scale, DWORD color)
{
	RESULT r = m_fontMan->DrawText(font, text, text_rect, attributes, scale, color);
	return r;
}

RESULT UserInterface::Think()
{
	input->Think();
	POINT p;
	POINT ap;
	POINT p2;
	POINT windowPos;
	GetMousePos(&p);
	cursor->SetPos(&p);
	cursor->Think();
	bool mouse_1 = GetMouseState(0);
	//int borderHit;
	SIZE s;
	if(!mouse_1)
	{
		m_attached_window = NULL;
		m_mouse_down1 = false;
		m_attached_window_border_hit = false;
	}

	if((mouse_1 & m_mouse_down1) && (m_attached_window))
	{
		ap = m_attached_window->GetAttachPoint();
		p2.x = p.x - ap.x;
		p2.y = p.y - ap.y;
		windowPos = m_attached_window->GetPos();
		if(m_attached_window->TitlebarHitTest(ap.x+windowPos.x, ap.y+windowPos.y))
			m_attached_window->Move(p2.x, p2.y);
		if(!m_attached_window_border_hit)
			m_borderHit = m_attached_window->BorderHitTest(p.x, p.y);
		if(m_borderHit)
		{
			m_attached_window_border_hit = true;
			if(m_borderHit == 3)
			{
				s = m_attached_window->Size();
				windowPos = m_attached_window->GetPos();
				s.cx = p.x - windowPos.x;
				if(s.cx < 25)
					s.cx = 25;
				m_attached_window->Resize(s.cx, s.cy);
				//ap.x = s.cx;
				//ap.y = s.cy;
				//m_attached_window->SetAttachPoint(ap);
			}
			if(m_borderHit == 4)
			{
				s = m_attached_window->Size();
				windowPos = m_attached_window->GetPos();
				s.cy = p.y - windowPos.y;
				if(s.cy < 25)
					s.cy = 25;
				m_attached_window->Resize(s.cx, s.cy);
			}
			if(m_borderHit == 7)
			{
				s = m_attached_window->Size();
				windowPos = m_attached_window->GetPos();
				s.cx = p.x - windowPos.x;
				if(s.cx < 25)
					s.cx = 25;
				s.cy = p.y - windowPos.y;
				if(s.cy < 25)
					s.cy = 25;
				m_attached_window->Resize(s.cx, s.cy);
			}
		}

	}
	if(mouse_1 & (!m_mouse_down1))
	{
		m_mouse_down1 = mouse_1;
		for(std::list<UIWindow*>::iterator i = windows.begin(); i != windows.end(); i++)
		{
			if(!(*i)->Active())
				continue;
			if((*i)->TitlebarHitTest(p.x, p.y))
			{
				ap = (*i)->GetPos();
				ap.x = p.x - ap.x;
				ap.y = p.y - ap.y;
				(*i)->SetAttachPoint(ap);
				m_active_window = m_attached_window = (*i);
				break;
			}
			m_borderHit = (*i)->BorderHitTest(p.x, p.y);
			if(m_borderHit)
			{
				ap = (*i)->GetPos();
				ap.x = p.x - ap.x;
				ap.y = p.y - ap.y;
				(*i)->SetAttachPoint(ap);
				m_active_window = m_attached_window = (*i);
				break;
			}
		}	
	}
	return OK;
}

bool UserInterface::GetKeyState(unsigned long keyId)
{
	return input->GetKey(keyId);
}

bool UserInterface::GetMouseState(unsigned long mouseBttn)
{
	return input->GetMouseButton(mouseBttn);
}

RESULT UserInterface::GetMousePos(POINT* p)
{
	return input->GetMousePos(p);
}

RESULT UserInterface::Render()
{
	// draw windows, etc
	cursor->Render();
	return OK;
}

RESULT UserInterface::SetInputHandler(IInputHandler* iih)
{
	return input->SetInputHandler(iih);
}

VOID UserInterface::SetCursor(ITexture* c)
{
	cursor->SetCursor(c);
}

VOID UserInterface::SetCursorInfo(std::string& ci)
{
	cursor->SetCursorInfo(ci);
}

char UserInterface::KeyToChar(DWORD keyConst, bool shift)
{
	return input->KeyToChar(keyConst, shift);
}

void UserInterface::AttachWindow(UIWindow* window)
{
	window->SetFontManager(m_fontMan);
	windows.push_back(window);
}

void UserInterface::DetachWindow(UIWindow* window)
{
	windows.remove(window);
}

UIWindow* UserInterface::GetWindow(moniker& id)
{
	for(std::list<UIWindow*>::iterator i = windows.begin(); i != windows.end(); i++)
	{
		if((*i)->Identifier()() == id())
			return *i;
	}
	return NULL;
}

char UserInterface::StringToKeyConst(std::string& keyname)
{
#define CONVERT(x, y) if(keyname == x) return (char)y;
	CONVERT("esc", DIV_KEY_ESCAPE);
	CONVERT("1", DIV_KEY_1);
	CONVERT("2", DIV_KEY_2);
	CONVERT("3", DIV_KEY_3);
	CONVERT("4", DIV_KEY_4);
	CONVERT("5", DIV_KEY_5);
	CONVERT("6", DIV_KEY_6);
	CONVERT("7", DIV_KEY_7);
	CONVERT("8", DIV_KEY_8);
	CONVERT("9", DIV_KEY_9);
	CONVERT("0", DIV_KEY_0);
	CONVERT("-", DIV_KEY_MINUS);
	CONVERT("=", DIV_KEY_EQUALS);
	CONVERT("backspace", DIV_KEY_BACK);
	CONVERT("tab", DIV_KEY_TAB);
	CONVERT("q", DIV_KEY_Q);
	CONVERT("w", DIV_KEY_W);
	CONVERT("e", DIV_KEY_E);
	CONVERT("r", DIV_KEY_R);
	CONVERT("t", DIV_KEY_T);
	CONVERT("y", DIV_KEY_Y);
	CONVERT("u", DIV_KEY_U);
	CONVERT("i", DIV_KEY_I);
	CONVERT("o", DIV_KEY_O);
	CONVERT("p", DIV_KEY_P);
	CONVERT("[", DIV_KEY_LBRACKET);
	CONVERT("]", DIV_KEY_RBRACKET);
	CONVERT("return", DIV_KEY_RETURN);
	CONVERT("l_ctrl", DIV_KEY_LCONTROL);
	CONVERT("a", DIV_KEY_A);
	CONVERT("s", DIV_KEY_S);
	CONVERT("d", DIV_KEY_D);
	CONVERT("f", DIV_KEY_F);
	CONVERT("g", DIV_KEY_G);
	CONVERT("h", DIV_KEY_H);
	CONVERT("j", DIV_KEY_J);
	CONVERT("k", DIV_KEY_K);
	CONVERT("l", DIV_KEY_L);
	CONVERT(";", DIV_KEY_SEMICOLON);
	CONVERT("'", DIV_KEY_APOSTROPHE);
	CONVERT("`", DIV_KEY_GRAVE);
	CONVERT("l_shift", DIV_KEY_LSHIFT);
	CONVERT("\\", DIV_KEY_BACKSLASH);
	CONVERT("z", DIV_KEY_Z);
	CONVERT("x", DIV_KEY_X);
	CONVERT("c", DIV_KEY_C);
	CONVERT("v", DIV_KEY_V);
	CONVERT("b", DIV_KEY_B);
	CONVERT("n", DIV_KEY_N);
	CONVERT("m", DIV_KEY_M);
	CONVERT(",", DIV_KEY_COMMA);
	CONVERT(".", DIV_KEY_PERIOD);
	CONVERT("/", DIV_KEY_SLASH);
	CONVERT("r_shift", DIV_KEY_RSHIFT);
	CONVERT("num_*", DIV_KEY_MULTIPLY);
	CONVERT("l_alt", DIV_KEY_LMENU);
	CONVERT("space", DIV_KEY_SPACE);
	CONVERT("f1", DIV_KEY_F1);
	CONVERT("f2", DIV_KEY_F2);
	CONVERT("f3", DIV_KEY_F3);
	CONVERT("f4", DIV_KEY_F4);
	CONVERT("f5", DIV_KEY_F5);
	CONVERT("f6", DIV_KEY_F6);
	CONVERT("f7", DIV_KEY_F7);
	CONVERT("f8", DIV_KEY_F8);
	CONVERT("f9", DIV_KEY_F9);
	CONVERT("f10", DIV_KEY_F10);
	CONVERT("num_7", DIV_KEY_NUMPAD7);
	CONVERT("num_8", DIV_KEY_NUMPAD8);
	CONVERT("num_9", DIV_KEY_NUMPAD9);
	CONVERT("num_-", DIV_KEY_SUBTRACT);
	CONVERT("num_4", DIV_KEY_NUMPAD4);
	CONVERT("num_5", DIV_KEY_NUMPAD5);
	CONVERT("num_6", DIV_KEY_NUMPAD6);
	CONVERT("num_+", DIV_KEY_ADD);
	CONVERT("num_1", DIV_KEY_NUMPAD1);
	CONVERT("num_2", DIV_KEY_NUMPAD2);
	CONVERT("num_3", DIV_KEY_NUMPAD3);
	CONVERT("num_0", DIV_KEY_NUMPAD0);
	CONVERT("num_.", DIV_KEY_DECIMAL);
	CONVERT("f11", DIV_KEY_F11);
	CONVERT("f12", DIV_KEY_F12);
	CONVERT("f13", DIV_KEY_F13);
	CONVERT("f14", DIV_KEY_F14);
	CONVERT("f15", DIV_KEY_F15);
	CONVERT("num_enter", DIV_KEY_NUMPADENTER);
	CONVERT("r_ctrl", DIV_KEY_RCONTROL);
	CONVERT("num_/", DIV_KEY_DIVIDE);
	CONVERT("r_alt", DIV_KEY_RMENU);
	CONVERT("home", DIV_KEY_HOME);
	CONVERT("up", DIV_KEY_UP);
	CONVERT("pgup", DIV_KEY_PRIOR);
	CONVERT("left", DIV_KEY_LEFT);
	CONVERT("right", DIV_KEY_RIGHT);
	CONVERT("end", DIV_KEY_END);
	CONVERT("down", DIV_KEY_DOWN);
	CONVERT("pgdn", DIV_KEY_NEXT);
	CONVERT("insert", DIV_KEY_INSERT);
	CONVERT("delete", DIV_KEY_DELETE);
	CONVERT("mouse_1_dn", DIV_KEY_MOUSE1_DOWN);
	CONVERT("mouse_1_up", DIV_KEY_MOUSE1_UP);
	CONVERT("mouse_2_dn", DIV_KEY_MOUSE2_DOWN);
	CONVERT("mouse_2_up", DIV_KEY_MOUSE2_UP);
	CONVERT("mouse_3_dn", DIV_KEY_MOUSE3_DOWN);
	CONVERT("mouse_3_up", DIV_KEY_MOUSE3_UP);
	CONVERT("mouse_4_dn", DIV_KEY_MOUSE4_DOWN);
	CONVERT("mouse_4_up", DIV_KEY_MOUSE4_UP);
	CONVERT("mouse_scroll_dn", DIV_KEY_MOUSESCROLL_DOWN);
	CONVERT("mouse_scroll_up", DIV_KEY_MOUSESCROLL_UP);
	
	
	
	return 0;
#undef CONVERT
}

void UserInterface::UnselectWindow()
{
	m_active_window = NULL;
}