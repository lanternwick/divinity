#ifndef UIGAMEQUITBUTTON_H
#define UIGAMEQUITBUTTON_H

#include "..\DivUserInterface\UIButton.h"

class UIGameQuitButton : public UIButton
{
public:
	UIGameQuitButton(char* id);
	virtual bool Mouse_ButtonUp(int button);
	void SetRunningPointer(bool* running);
private:
	UIGameQuitButton();
	bool* m_running;

};

#endif