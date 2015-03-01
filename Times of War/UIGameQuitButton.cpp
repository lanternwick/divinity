#include "UIGameQuitButton.h"

UIGameQuitButton::UIGameQuitButton()
{

}

UIGameQuitButton::UIGameQuitButton(char* id)
{
	m_id() = id;
}

bool UIGameQuitButton::Mouse_ButtonUp(int button)
{
	*m_running = false;
	return true;
}

void UIGameQuitButton::SetRunningPointer(bool* running)
{
	m_running = running;
}