#ifndef _IINDEXBUFFER_H
#define _IINDEXBUFFER_H

#include <CObject.h>
#include "SYSDevice.h"

class IIndexBuffer : public CObject
{
public:
	IIndexBuffer();
	virtual ~IIndexBuffer()=0;

	virtual RESULT Create(SYSDevice* Device, UINT numIndices, DWORD usage)=0;
	virtual RESULT Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)=0;
	virtual RESULT Unlock()=0;
	virtual RESULT Set(SYSDevice* Device)=0;
	virtual RESULT Release()=0;
	virtual VOID ResetDeviceObjects()=0;

private:
	UINT mySize;
	DWORD myUsage;
	BYTE* myData;
	UINT myLockCount;
	SYSDevice* myDevice;
};


#endif