#ifndef _IVERTEXBUFFER_H
#define _IVERTEXBUFFER_H

// just check over each of these functions, to make sure they work as desired.

#include "..\Common\IObject.h"
#include "..\Common\Result.h"
#include "..\Common\CSmartPtr.h"
#include "D3DDevice.h"

class _API IVertexBuffer : public IObject
{
public:
	IVertexBuffer(char* id)
	{
		_identifier = NULL;
		_identifier = new char[64];
		strcpy(_identifier, id);
	}
	virtual ~IVertexBuffer(){}
	
	virtual RESULT Create(CSmartPtr<IDevice> Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF)=0;
	
	virtual RESULT Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)=0;
	virtual RESULT Unlock()=0;
	virtual RESULT Set(CSmartPtr<IDevice> Device)=0;
	virtual void Release()=0;
	virtual VOID ResetDeviceObjects()=0;
protected:
	BYTE* myData;
	BYTE* savedP;
	INT myLockCount;
	DWORD myFVF;
	DWORD myVertSize;
	DWORD myUsage;
	DWORD mySize;
	CSmartPtr<IDevice> myDev;
	IVertexBuffer(){};
};

_EXTERN_ template class _API CSmartPtr<IVertexBuffer>;

#endif

		
