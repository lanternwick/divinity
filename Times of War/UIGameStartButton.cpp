#include "UIGameStartButton.h"

UIGameStartButton::UIGameStartButton()
{

}

UIGameStartButton::UIGameStartButton(char* id)
{
	m_id() = id;
}

bool UIGameStartButton::Mouse_ButtonUp(int button)
{
	*m_mode = 0;
	return true;
}

void UIGameStartButton::SetModePointer(int* mode)
{
	m_mode = mode;
}