#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <list>
#include "UIWindow.h"
#include "IInputDevice.h"
#include "IDevice.h"
#include "gwen.h"

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	moniker& UICreateWindow(window_def& definition);
	UIWindow& Window(moniker& window_name);
	UIWindow* ActiveWindow();
	void AttachWindow(UIWindow* window);
	void ActiveWindow(UIWindow* new_act);

	int UpdateAttachedWindows(IInputDevice* input);
	typedef std::list<UIWindow*>::iterator window_iter;

	// generator functions
	int GenerateGUI(const std::string& file);
	int gen_gui(g_block<char>& b, UIWindow* working_window);
	UIWindow* gen_ui_window(g_block<char>& b, UIWindow* working_window);
	UIWindow* gen_ui_button(g_block<char>& b, UIWindow* working_window);
	UIWindow* gen_ui_textbox(g_block<char>& b, UIWindow* working_window);
	UIWindow* gen_ui_scrollbar(g_block<char>& b, UIWindow* working_window);
	UIWindow* gen_ui_displaybox(g_block<char>& b, UIWindow* working_window);
	UIWindow* gen_ui_checkbox(g_block<char>& b, UIWindow* working_window);
	//iterator functions
	window_iter Iterator_Begin();
	window_iter Iterator_End();

	int set_device(IDevice* device);


private:
	std::list<UIWindow*> m_window_list;
	UIWindow* m_active_window;
	IDevice* m_device;
};

#endif
