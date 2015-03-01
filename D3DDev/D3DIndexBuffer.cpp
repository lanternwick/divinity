#include "D3DIndexBuffer.h"

D3DIndexBuffer::D3DIndexBuffer(char* id)
{
	Core::Function_Enter("D3DIndexBuffer::Constructor(char*)");
	myData = NULL;
	myD3DIB = NULL;
	myLockCount = 0;
	if(id)
		m_id() = id;
	myDevice = NULL;
	Core::Function_Exit();
}


D3DIndexBuffer::D3DIndexBuffer()
{
	Core::Function_Enter("D3DIndexBuffer::Constructor()");
	myD3DIB = NULL;
	myData = NULL;
	myLockCount = 0;
	myDevice = NULL;
	Core::Function_Exit();
	//type = OBJ_INDEXBUFFER;
}

D3DIndexBuffer::~D3DIndexBuffer()
{
	Core::Function_Enter("D3DIndexBuffer::Destructor()");
	Release();
	Core::Function_Exit();
}

RESULT D3DIndexBuffer::Create(IDevice* Device, UINT numIndices, DWORD usage, BYTE type)
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
}

RESULT D3DIndexBuffer::Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)
{
	// lock the buffer to set data
	Core::Function_Enter("D3DIndexBuffer::Lock()");
	++myLockCount;
	RESULT r = HRtoR(myD3DIB->Lock(offsetToLock, sizeToLock, ptr, flags));
	myPtr = *ptr;
	Core::Function_Exit();
	return r;
	
}

RESULT D3DIndexBuffer::Unlock()
{
	// unlock the buffer
	Core::Function_Enter("D3DIndexBuffer::Unlock()");
	--myLockCount;
	memcpy(myData, myPtr, mySize*(myType==16 ? 2 : 4));
	RESULT r = HRtoR(myD3DIB->Unlock());
	// save the data;
	
	Core::Function_Exit();
	return r;
}

RESULT D3DIndexBuffer::Set()
{
	// set the buffer to be rendered from
	Core::Function_Enter("D3DIndexBuffer::Set()");
	RESULT r = myDevice->SetIndices((VOID*)myD3DIB, 0);
	Core::Function_Exit();
	return r;
}

void D3DIndexBuffer::Release()
{
	// kill the buffer
	Core::Function_Enter("D3DIndexBuffer::Release()");
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
	Core::Function_Exit();

	return;
}

VOID D3DIndexBuffer::ResetDeviceObjects()
{
	// recreate this buffer
	Core::Function_Enter("D3DIndexBuffer::ResetDeviceObjects()");
	
	Create(myDevice, mySize, myUsage, myType);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*(myType==16 ? 2 : 4));
	Unlock();
	Core::Function_Exit();
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