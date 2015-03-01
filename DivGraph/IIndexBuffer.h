#ifndef _IINDEXBUFFER_H
#define _IINDEXBUFFER_H

#include "..\Common\IObject.h"
#include "..\Common\CSmartPtr.h"
#include "IDevice.h"

class IIndexBuffer : public IObject
{
public:
	IIndexBuffer(char* id)
	{
		_identifier = NULL;
		_identifier = new char[64];
		strcpy(_identifier, id);
	}
	virtual ~IIndexBuffer(){}

	virtual RESULT Create(CSmartPtr<IDevice> Device, UINT numIndices, DWORD usage, BYTE type)=0;
	virtual RESULT Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)=0;
	virtual RESULT Unlock()=0;
	virtual RESULT Set(CSmartPtr<IDevice> Device)=0;
	virtual void Release()=0;
	virtual VOID ResetDeviceObjects()=0;

protected:
	UINT mySize;
	DWORD myUsage;
	BYTE* myData;
	UINT myLockCount;
	CSmartPtr<IDevice> myDevice;
	BYTE myType;
	BYTE* myPtr;
	IIndexBuffer(){};
};

_EXTERN_ template class _API CSmartPtr<IIndexBuffer>;

#endif