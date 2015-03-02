#ifndef _IMEDIAOBJECT_H
#define _IMEDIAOBJECT_H

#include "IObject.h"

class IMediaObject : public IObject
{
public:
	IMediaObject(char* id);
	virtual ~IMediaObject();

	virtual void Release();
	virtual void Release_SaveData();
	virtual void ResetDeviceObjects();
protected:
	IMediaObject();

};



#endif
