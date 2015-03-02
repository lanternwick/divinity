#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <list>
#include "UIWindow.h"

class _API WindowManager
{
public:
	WindowManager();
	~WindowManager();

	moniker& UICreateWindow(window_def& definition);
	UIWindow& Window(moniker& window_name);
	UIWindow* ActiveWindow();
	void AttachWindow(UIWindow* window);
	void ActiveWindow(UIWindow* new_act);

	typedef std::list<UIWindow*>::iterator window_iter;

	//iterator functions
	window_iter Iterator_Begin();
	window_iter Iterator_End();
private:
	std::list<UIWindow*> m_window_list;
	UIWindow* m_active_window;
};

#endif
