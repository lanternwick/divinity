#ifndef _IVERTEXBUFFER_H
#define _IVERTEXBUFFER_H

// just check over each of these functions, to make sure they work as desired.

#include "IObject.h"
#include "Result.h"
#include "IMediaObject.h"

class IDevice;

class IVertexBuffer : public IMediaObject
{
public:
	IVertexBuffer(const char* id)
	{
		m_id() = id;
	}
	virtual ~IVertexBuffer(){}
	
	//virtual RESULT Create(IDevice* Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF)=0;
	
	virtual RESULT Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)=0;
	virtual RESULT Unlock()=0;
	virtual RESULT Set()=0;
	virtual void Release()=0;
	virtual VOID ResetDeviceObjects()=0;
	static VOID MakeNull(IDevice* device);

protected:
	BYTE* myData;
	BYTE* savedP;
	INT myLockCount;
	DWORD myFVF;
	DWORD myVertSize;
	DWORD myUsage;
	DWORD mySize;
	IDevice* myDev;
	IVertexBuffer(){};
};



#endif

		
