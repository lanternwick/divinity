#ifndef _IINDEXBUFFER_H
#define _IINDEXBUFFER_H

#include "IObject.h"
#include "Result.h"
#include "IMediaObject.h"

class IDevice;

class IIndexBuffer : public IMediaObject
{
public:
	IIndexBuffer(const char* id)
	{
		m_id = id;
	}
	virtual ~IIndexBuffer(){}

	//virtual RESULT Create(IDevice* Device, UINT numIndices, DWORD usage, BYTE type)=0;
	virtual RESULT Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)=0;
	virtual RESULT Unlock()=0;
	virtual RESULT Set()=0;
	virtual void Release()=0;
	virtual VOID ResetDeviceObjects()=0;

protected:
	UINT mySize;
	DWORD myUsage;
	BYTE* myData;
	UINT myLockCount;
	IDevice* myDevice;
	BYTE myType;
	BYTE* myPtr;
	IIndexBuffer(){};
};



#endif