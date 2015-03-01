#ifndef _IMEDIAOBJECT_H
#define _IMEDIAOBJECT_H

#include "..\Common\IObject.h"

class _API IMediaObject : public IObject
{
public:
	IMediaObject(char* id);
	~IMediaObject();

	virtual void Release();
protected:
	IMediaObject();

};

_EXTERN_ template class _API CSmartPtr<IMediaObject>;

#endif
