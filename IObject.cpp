#include "IObject.h"

_log* IObject::m_log = NULL;

IObject::IObject()
{
	m_id() = "";
}

IObject::IObject(const char* id)
{
	m_id = moniker(id);
}

IObject::~IObject()
{
	Release();
}

void IObject::Release()
{
	
}

moniker IObject::Identifier() const
{
	return m_id;
}

#ifdef LOGME
int IObject::set_log(_log* l)
{
	m_log = l;
	return 0;
}
#endif
