#include "IObject.h"

IObject::IObject()
{
	_identifier = NULL;
}

IObject::IObject(char* id)
{
	_identifier = NULL;
	_identifier = new char[64];
	strcpy(_identifier, id);
}

IObject::~IObject()
{
	Release();
}

void IObject::Release()
{
	if(_identifier)
		delete [] _identifier;
}

char* IObject::Identifier() const
{
	return _identifier;
}


