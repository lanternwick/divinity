#ifndef _TOW_LOGPRINT_H
#define _TOW_LOGPRINT_H

/* Deprecated.  If its here, I just never deleted it */

#include "..\common\Irenderable.h"
#include "..\Common\Result.h"

#include "..\DivUserInterface\Userinterface.h"
#include <windows.h>
#include <string>

class TOW_LogPrint : public IRenderable
{
public:
	TOW_LogPrint(IDevice* device, UserInterface* ui, moniker& font_name, DWORD color, RECT font_rect);
	void SetScene(CScene* scene);
	RESULT Render();
private:
	moniker m_font;
	DWORD m_color;
	UserInterface* m_ui;
	IDevice* m_device;
	RECT m_rect;
	CScene* m_scene;

};

#endif