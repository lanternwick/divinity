#ifndef _IOBJECT_H
#define _IOBJECT_H

#ifdef LOGME
#include "log_stream.h"
#endif

#include "custmoniker.h"

#include <string>

class IObject
{
public:
	IObject(const char* id);
	virtual ~IObject();

	moniker Identifier() const;
	virtual void Release();
#ifdef LOGME
	static int set_log(_log* l);
#endif
protected:
	moniker m_id;
	IObject();
#ifdef LOGME
	static _log* m_log;
#endif
};




#endif




