#ifndef _UICURSOR_H
#define	_UICURSOR_H


#include "..\DivGraph\DivGraph.h"
#include "..\Common\Result.h"
#include "UIWindow.h"
#include "..\Common\define_exports.h"


class _API UICursor : public UIWindow
{
public:
	UICursor();
	~UICursor();

	RESULT Render();
	RESULT Think();
	RESULT Create(IDevice* device, IGraphObjFactory* factory);
	RESULT SetPos(POINT* p);
	RESULT SetCursor(moniker& cursor);
	bool active();
private:
	IVertexBuffer* m_vb;
	POINT pos;
	IDevice* m_device;
	moniker m_cursor;
	void Update();

};

#endif
