#ifndef _USERINTERFACE_H
#define _USERINTERFACE_H

#include <windows.h>
#include "DivUserInterface.h"
#include "DIDevice.h"
#include "..\Common\define_exports.h"
#include "FontManager.h"
#include "..\Common\IEntity.h"
#include "UICursor.h"
#include "UIWindow.h"
#include <list>
#include "IInputDevice.h"

class IInputHandler;
class UIInputHandler;

class _API UserInterface : public IEntity
{
	friend class UIInputHandler;	
public:
	UserInterface();
	virtual ~UserInterface();

	static UserInterface* Instance();
	
	RESULT Initialize(HWND hWnd, IDevice* device, IGraphObjFactory* factory);
	
	RESULT LoadFont(char* filename, moniker& fontname);
	RESULT RemoveFont(moniker& fontname);
	RESULT DrawText(const moniker& font, const char* text, RECT text_rect, char* attributes, float scale, DWORD color);
	RESULT Think();
	bool GetKeyState(unsigned long keyId);
	bool GetMouseState(unsigned long mouseBttn);
	RESULT GetMousePos(POINT* p);
	RESULT SetInputHandler(IInputHandler* iih);
	RESULT Render();
	VOID SetCursor(ITexture* c);
	VOID SetCursorInfo(std::string& ci);

	char KeyToChar(DWORD keyConst, bool shift);
	char StringToKeyConst(std::string& keyname);
	void AttachWindow(UIWindow* window);
	void DetachWindow(UIWindow* window);
	UIWindow* GetWindow(moniker& id);
	void UnselectWindow();
	void DeactivateWindow(moniker& m);
	void ActivateWindow(moniker& m);
	typedef std::list<_API UIWindow*>::iterator window_iter;
	
private:
	
	
	static UserInterface* _instance;
	IDevice* m_device;
	IGraphObjFactory* m_factory;
	FontManager* m_fontMan;
	IInputDevice* input;
	UICursor* cursor;
	
	std::list<_API UIWindow*> windows;
	bool m_mouse_down1;
	bool m_mouse_down2;
	UIWindow* m_attached_window;
	bool m_attached_window_border_hit;
	int m_borderHit;
	UIWindow* m_active_window;
	
};

#endif