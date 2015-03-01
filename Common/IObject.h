#ifndef _IOBJECT_H
#define _IOBJECT_H

#define NULL 0

#include "define_exports.h"
#include "..\Common\custmoniker.h"

#include <string.h>

class _API IObject
{
public:
	IObject(char* id);
	virtual ~IObject();

	moniker Identifier() const;
	virtual void Release();
protected:
	moniker m_id;
	IObject();
};




#endif




