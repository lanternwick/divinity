#ifndef UICONSOLEWND_H
#define UICONSOLEWND_H

#include "UIWindow.h"
#include "UserInterface.h"
#include "..\DivCore\ITimer.h"

class _API UIConsoleWnd : public UIWindow, public ITimer
{
public:
	
	UIConsoleWnd(char* id);
	virtual ~UIConsoleWnd();

	RESULT Think();
	void SetState(int state);
	int GetState();
	void On_Timer(float t);
	RESULT Render();
	void SetUIPtr(UserInterface* ui);
	void SetTextColor(unsigned long color);
	void SetPrompt(std::string& p);
	virtual void AddKeyToBuffer(char c);
protected:
	UIConsoleWnd();
	int m_state;
	UserInterface* m_ui;
	unsigned long m_textColor;
	const float m_scale;
	std::string m_prompt;
};

#endif