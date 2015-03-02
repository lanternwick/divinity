#include "WindowManager.h"
#include "gwen.h"

WindowManager::WindowManager()
{
	m_window_list.push_front(NULL);
}

WindowManager::~WindowManager()
{
	while(!m_window_list.empty())
	{
		delete m_window_list.back();
		m_window_list.pop_back();
	}
}

moniker& WindowManager::UICreateWindow(window_def& definition)
{
	
	return definition.window_name;
}

UIWindow& WindowManager::Window(moniker& window_name)
{
	for(std::list<UIWindow*>::iterator i = m_window_list.begin(); i != m_window_list.end(); ++i)
	{
		if(!(*i))
		{
			continue;
		}
		if((*i)->window_name()() == window_name())
		{
			return *(*i);
		}
	}
	throw;
	
}

UIWindow* WindowManager::ActiveWindow()
{
	if(!m_window_list.empty())
	{
		return m_window_list.front();
	}
	return NULL;
}

void WindowManager::ActiveWindow(UIWindow* new_act)
{
	UIWindow* old_act = ActiveWindow();
	m_window_list.remove(new_act);
	if(!old_act)
	{
		m_window_list.remove(NULL);
	}
	else
	{
		old_act->Unfocus();
	}
	m_window_list.push_front(new_act);
	if(new_act)
	{
		new_act->Focus();
	}
}

void WindowManager::AttachWindow(UIWindow* window)
{
	m_window_list.push_back(window);
}

int WindowManager::GenerateGUI(const std::string& file)
{
	std::string gui_def("gui_def");
	std::string element("element");
	file_g_input<char> fi(file);
	g_parser<char> p(fi);
	g_block<char> cb;
	
	try
	{
		g_block<char> root = p.root().block(gui_def);
		
		for(int i = 0; i < root.num_blocks_of_type(element); ++i)
		{
			cb = root.block(element, i);
			gen_gui(cb, NULL);
		}
		
	}
	catch(g_exception e)
	{
		return 0;	
	}



	return 0;
}

int WindowManager::gen_gui(g_block<char>& b, UIWindow* working_window)
{
	std::string name("name");
	std::string n;
	g_block<char> cb;
	UIWindow* win = working_window;
	if(b.type() == "element")
	{
		if(b.attribute_exists(name))
		{
			n = b.attribute(name);
		}
		else
		{
			return 0;
		}
		if(n == "window")
		{
			working_window = gui_gen_window(b, working_window);
		}
		if(n == "button")
		{
			working_window = gui_gen_button(b, working_window);
		}
		if(n == "textbox")
		{
			working_window = gui_gen_textbox(b, working_window);
		}
		if(n == "scrollbar")
		{
			working_window = gui_gen_scrollbar(b, working_window);
		}
		if(n == "displaybox")
		{
			working_window = gui_gen_displaybox(b, working_window);
		}
		if(n == "checkbox")
		{
			working_window = gui_gen_checkbox(b, working_window);
		}
	}
	if(b.type() == "decoration")
	{
		// decoration for parent node
	}
	if(b.type() == "attach")
	{
		// attach this element to a window
	}
	for(int i = 0; i < b.num_blocks(); ++i)
	{
		cb = root.block(i);
		gen_gui(cb, win);
	}
	return 0;
}

UIWindow* WindowManager::gen_ui_window(g_block<char>& b, UIWindow* working_window)
{
	UIBaseWindow* bw = new UIBaseWindow;
	float xp, yp, cxp, cyp;
	std::string color_s;
	std::string tex_name;
	ITexture* tex;
	unsigned long color;
	GSS(b, "x") >> xp;
	GSS(b, "y") >> yp;
	GSS(b, "cx") >> cxp;
	GSS(b, "cy") >> cyp;
	GSS(b, "color") >> color_s;
	GSS(b, "texture") >> tex_name;
	color = s_to_color(color_s.c_str());
	m_device->LoadTexture(tex_name.c_str(), tex);
	bw->Create(m_device, xp, yp, cxp, cyp, color, tex, working_window);
	return bw;
}

UIWindow* WindowManager::gen_ui_button(g_block<char>& b, UIWindow* working_window)
{

}

UIWindow* WindowManager::gen_ui_textbox(g_block<char>& b, UIWindow* working_window)
{

}

UIWindow* WindowManager::gen_ui_scrollbar(g_block<char>& b, UIWindow* working_window)
{

}

UIWindow* WindowManager::gen_ui_displaybox(g_block<char>& b, UIWindow* working_window)
{

}

UIWindow* WindowManager::gen_ui_checkbox(g_block<char>& b, UIWindow* working_window)
{

}

WindowManager::window_iter WindowManager::Iterator_Begin()
{
	return m_window_list.begin();
}

WindowManager::window_iter WindowManager::Iterator_End()
{
	return m_window_list.end();
}

int WindowManager::UpdateAttachedWindows(IInputDevice* input)
{
	int ox;
	int oy;
	POINT p;
	input->GetMousePos(&p);

	for(std::list<UIWindow*>::iterator i = m_window_list.begin(); i != m_window_list.end(); ++i)
	{
		if(!(*i))
		{
			continue;
		}
		if((*i)->IsAttachedToMouse())
		{
			(*i)->GetMouseOffset(ox, oy);
			(*i)->x(p.x-ox);
			(*i)->y(p.y-oy);
		}
	}
	return 0;
}

int WindowManager::set_device(IDevice* device)
{
	m_device = device;
	return 0;
}