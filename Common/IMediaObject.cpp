#include "IMediaObject.h"

IMediaObject::IMediaObject()
{

}

IMediaObject::IMediaObject(char* id)
{
	_identifier = NULL;
	_identifier = new char[64];
	strcpy(_identifier, id);
}

IMediaObject::~IMediaObject()
{
	Release();
}

void IMediaObject::Release()
{

}
