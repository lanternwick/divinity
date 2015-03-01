#include "D3DVertexBuffer.h"

D3DVertexBuffer::D3DVertexBuffer(char* id)
{
	IVertexBuffer::IVertexBuffer(id);
	D3DVertexBuffer();
}

D3DVertexBuffer::D3DVertexBuffer()
{
	//type = OBJ_VERTEXBUFFER;
	myLockCount = 0;
	myD3DVB = NULL;
}

D3DVertexBuffer::~D3DVertexBuffer()
{
	Release();
}

RESULT D3DVertexBuffer::Create(CSmartPtr<IDevice> Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF)
// creates a vertex buffer

{
	// save parameters
	myFVF = FVF;
	myVertSize = VertSize;
	mySize = numVerts;
	myUsage = Usage;
	myFVF = FVF;
	// new buffer
	myData = new BYTE[numVerts*VertSize];
	myDev = Device;
	// create the buffer
	Device->CreateVertexBuffer(numVerts, VertSize, Usage, FVF,  (VOID**)&myD3DVB);
	if(myD3DVB == NULL)
		return OUTOFMEMORY;
	return OK;
	
}

RESULT D3DVertexBuffer::Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)
{
	// lock the buffer
	RESULT r;
	++myLockCount;
	if(myD3DVB == NULL)
	{
		--myLockCount;
		return VBNOTCREATED;
	}
	r = HRtoR(myD3DVB->Lock(OffsetToLock, SizeToLock, ptr, Flags));
	savedP = *ptr;
	return r;
}

RESULT D3DVertexBuffer::Unlock()
{
	// unlock the buffer
	HRESULT hr;  
	if(myD3DVB == NULL)
		return VBNOTCREATED;
	hr = myD3DVB->Unlock();
	if(SUCCEEDED(hr))
		--myLockCount;
	else
		return HRtoR(hr);
	memcpy(myData, savedP, mySize); // and save the data
	return OK;
}

void D3DVertexBuffer::Release()
{
	// kill the buffer
	if(myD3DVB != NULL) myD3DVB->Release();
	myDev.Release();
}

RESULT D3DVertexBuffer::Set(CSmartPtr<IDevice> Device)
{
	// set this buffer to render from
	if(myD3DVB == NULL)
		return VBNOTCREATED;
	Device->SetVertexShader(myFVF);
	Device->SetVertexBuffer(myD3DVB, myVertSize);
	return OK;
}

VOID D3DVertexBuffer::ResetDeviceObjects()
{
	// recreate this buffer
	Release();
	Create(myDev, mySize, myVertSize, myUsage, myFVF);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*myVertSize);
	Unlock();
	return;
}