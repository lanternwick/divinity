#ifndef _UICURSOR_H
#define	_UICURSOR_H



#include "Result.h"
#include "UIWindow.h"

class UICursor : public UIWindow
{
public:
	UICursor();
	~UICursor();

	RESULT Render();
	RESULT Think();
	RESULT Create(IDevice* device, IInputDevice* input);
	RESULT SetPos(POINT* p);
	RESULT SetCursor(ITexture* cursor);
	bool active();
private:
	IVertexBuffer* m_vb;
	POINT pos;
	IDevice* m_device;
	ITexture* m_cursor;
	IInputDevice* m_input;
	void Update();

};

#endif
