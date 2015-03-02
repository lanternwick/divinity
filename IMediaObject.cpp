#include "IMediaObject.h"

IMediaObject::IMediaObject()
{

}

IMediaObject::IMediaObject(char* id)
{
	m_id = moniker(id);
}

IMediaObject::~IMediaObject()
{
	Release();
}

void IMediaObject::Release()
{

}

void IMediaObject::Release_SaveData()
{

}

void IMediaObject::ResetDeviceObjects()
{

}