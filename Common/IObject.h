#ifndef _IOBJECT_H
#define _IOBJECT_H

#define NULL 0

#include "define_exports.h"
#include "CSmartPtr.h"
#include <string.h>

class _API IObject
{
public:
	IObject(char* id);
	~IObject();

	char* Identifier() const;
	virtual void Release();
protected:
	char* _identifier;
	IObject();
};

_EXTERN_ template class _API CSmartPtr<IObject>;


#endif




