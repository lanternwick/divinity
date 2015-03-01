#include "D3DIndexBuffer.h"


D3DIndexBuffer::D3DIndexBuffer()
{
	myD3DIB = NULL;
	myLockCount = 0;
	type = OBJ_INDEXBUFFER;
}

D3DIndexBuffer::~D3DIndexBuffer()
{
	Release();
}

RESULT D3DIndexBuffer::Create(SYSDevice* Device, UINT numIndices, DWORD usage, BYTE type)
{
	myDevice = Device;
	mySize = numIndices;
	myUsage = usage;
	Device->CreateIndexBuffer(numIndices, usage, type, (VOID**)&myD3DIB);
	return OK;
}

RESULT D3DIndexBuffer::Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)
{
	++myLockCount;
	return HRtoR(myD3DIB->Lock(offsetToLock, sizeToLock, ptr, flags));
	
}

RESULT D3DIndexBuffer::Unlock()
{
	--myLockCount;
	return HRtoR(myD3DIB->Unlock());
}

RESULT D3DIndexBuffer::Set(SYSDevice* Device)
{
	return Device->SetIndices((VOID*)myD3DIB, 0);
}

RESULT D3DIndexBuffer::Release()
{
	if(myD3DIB != NULL)
	{
		myD3DIB->Release();
		myD3DIB = NULL;
	}
	return OK;
}

VOID D3DIndexBuffer::ResetDeviceObjects()
{
	Release();
	Create(myDevice, mySize, myUsage);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*sizeof(WORD));
	Unlock();
	return;
}