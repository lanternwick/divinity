#include "IObject.h"

IObject::IObject()
{
	m_id() = "";
}

IObject::IObject(char* id)
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


