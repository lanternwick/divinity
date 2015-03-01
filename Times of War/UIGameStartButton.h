#ifndef UIGAMESTARTBUTTON_H
#define UIGAMESTARTBUTTON_H

#include "..\DivUserInterface\UIButton.h"

class UIGameStartButton : public UIButton
{
public:
	UIGameStartButton(char* id);
	virtual bool Mouse_ButtonUp(int button);
	void SetModePointer(int* mode);
private:
	UIGameStartButton();
	int* m_mode;

};

#endif