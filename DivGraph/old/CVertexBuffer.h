#ifndef _CVERTEXBUFFER_H
#define _CVERTEXBUFFER_H

// just check over each of these functions, to make sure they work as desired.

#include <CObject.h>
#include <Result.h>
#include <d3dx8.h>
#include "SYSDevice.h"

enum API;

class IVertexBuffer : public CObject
{
public:
	IVertexBuffer();
	virtual ~IVertexBuffer()=0;
	
	virtual RESULT Create(SYSDevice* Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF)=0;
	
	virtual RESULT Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)=0;
	virtual RESULT Unlock()=0;
	virtual RESULT Set(SYSDevice* Device)=0;
	virtual RESULT Release()=0;
	virtual VOID ResetDeviceObjects()=0;
protected:
	BYTE* myData;
	BYTE* savedP;
	INT myLockCount;
	DWORD myFVF;
	DWORD myVertSize;
	DWORD myUsage;
	DWORD mySize;
	SYSDevice* myDev;
};

#endif

		
