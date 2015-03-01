#ifndef _UICURSOR_H
#define	_UICURSOR_H


#include "..\DivGraph\DivGraph.h"
#include "..\Common\Result.h"
#include "..\Common\IObject.h"
#include "..\Common\IEntity.h"
#include "..\Common\IThinker.h"
#include "Fontmanager.h"

#include <string>

class UICursor : IEntity
{
public:
	UICursor();
	~UICursor();

	RESULT Render();
	RESULT Think();
	RESULT Create(IDevice* device, IGraphObjFactory* factory, FontManager* fm);
	RESULT SetPos(POINT* p);
	RESULT SetCursor(ITexture* cursor);
	RESULT SetCursorInfo(std::string& ci);
private:
	IVertexBuffer* m_vb;
	POINT pos;
	IDevice* m_device;
	ITexture* m_cursor;
	FontManager* m_fontMan;
	std::string cursor_info;
	void Update();
};

#endif
