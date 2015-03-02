#include "D3DIndexBuffer.h"
#include "D3DDevice.h"

D3DIndexBuffer::D3DIndexBuffer(const char* id)
{
	
	myData = NULL;
	myD3DIB = NULL;
	myLockCount = 0;
	if(id)
		m_id() = id;
	myDevice = NULL;
}


D3DIndexBuffer::D3DIndexBuffer()
{
	myD3DIB = NULL;
	myData = NULL;
	myLockCount = 0;
	myDevice = NULL;
}

D3DIndexBuffer::~D3DIndexBuffer()
{
	Release();
}

/*RESULT D3DIndexBuffer::Create(IDevice* Device, UINT numIndices, DWORD usage, BYTE type)
{
	// create this index buffer
	Core::Function_Enter("D3DIndexBuffer::Create()");
	Device->RemoveFromResetList(this);
	myDevice = Device;
	mySize = numIndices;
	myUsage = usage;
	myType = type;
	Device->CreateIndexBuffer(this, numIndices, usage, type, (VOID**)&myD3DIB);
	if(myData == NULL)
		myData = new BYTE[numIndices*(type==16 ? 2 : 4)];
	Core::Function_Exit();
	return OK;
}*/

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
	memcpy(myData, myPtr, mySize*(myType==16 ? 2 : 4));
	RESULT r = HRtoR(myD3DIB->Unlock());
	// save the data;
	
	return r;
}

RESULT D3DIndexBuffer::Set()
{
	// set the buffer to be rendered from
	RESULT r = myDevice->SetIndices((VOID*)myD3DIB, 0);
	return r;
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
	if(myData) 
		delete myData;
	
	myData = NULL;
	if(myDevice)
		myDevice->RemoveFromResetList(this);

	return;
}

VOID D3DIndexBuffer::ResetDeviceObjects()
{
	// recreate this buffer	
	IIndexBuffer* t = this;
	myDevice->CreateIndexBuffer(mySize, myUsage, myType, t, NULL);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*(myType==16 ? 2 : 4));
	Unlock();
	
	return;
}

void D3DIndexBuffer::Release_SaveData()
{
	if(myD3DIB != NULL)
	{
		myDevice->SetIndices(NULL, 0);
		myD3DIB->Release();
		myD3DIB = NULL;
	}
}