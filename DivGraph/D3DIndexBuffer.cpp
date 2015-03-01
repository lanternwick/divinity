#include "D3DIndexBuffer.h"

D3DIndexBuffer::D3DIndexBuffer(char* id)
{
	IIndexBuffer::IIndexBuffer(id);
	D3DIndexBuffer();
}


D3DIndexBuffer::D3DIndexBuffer()
{
	myD3DIB = NULL;
	myLockCount = 0;
	//type = OBJ_INDEXBUFFER;
}

D3DIndexBuffer::~D3DIndexBuffer()
{
	Release();
}

RESULT D3DIndexBuffer::Create(CSmartPtr<IDevice> Device, UINT numIndices, DWORD usage, BYTE type)
{
	// create this index buffer
	myDevice = Device;
	mySize = numIndices;
	myUsage = usage;
	myType = type;
	Device->CreateIndexBuffer(numIndices, usage, type, (VOID**)&myD3DIB);
	myData = new BYTE[numIndices*(type==16 ? 2 : 4)];
	return OK;
}

RESULT D3DIndexBuffer::Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)
{
	// lock the buffer to set data
	++myLockCount;
	RESULT r = HRtoR(myD3DIB->Lock(offsetToLock, sizeToLock, ptr, flags));
	myPtr = *ptr;
	return r;
	
}

RESULT D3DIndexBuffer::Unlock()
{
	// unlock the buffer
	--myLockCount;
	RESULT r = HRtoR(myD3DIB->Unlock());
	// save the data;
	memcpy(myData, myPtr, mySize*(myType==16 ? 2 : 4));
	return r;
}

RESULT D3DIndexBuffer::Set(CSmartPtr<IDevice> Device)
{
	// set the buffer to be rendered from
	return Device->SetIndices((VOID*)myD3DIB, 0);
}

void D3DIndexBuffer::Release()
{
	// kill the buffer
	if(myD3DIB != NULL)
	{
		myDevice->SetIndices(NULL, 0);
		myD3DIB->Release();
		myD3DIB = NULL;
	}
	myDevice.Release();

	return;
}

VOID D3DIndexBuffer::ResetDeviceObjects()
{
	// recreate this buffer
	Release();
	Create(myDevice, mySize, myUsage, myType);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*(myType==16 ? 2 : 4));
	Unlock();
	return;
}