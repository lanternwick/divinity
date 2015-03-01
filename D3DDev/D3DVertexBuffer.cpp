#include "D3DVertexBuffer.h"

D3DVertexBuffer::D3DVertexBuffer(char* id)
{
	Core::Function_Enter("D3DVertexBuffer::Constructor(char*)");
	myLockCount = 0;
	myD3DVB = NULL;
	myData = NULL;
	myDev = NULL;
	if(id)
		m_id() = id;
	Core::Function_Exit();
}

D3DVertexBuffer::D3DVertexBuffer()
{
	Core::Function_Enter("D3DVertexBuffer::Constructor()");
	myLockCount = 0;
	myD3DVB = NULL;
	myData = NULL;
	myDev = NULL;
	Core::Function_Exit();
}

D3DVertexBuffer::~D3DVertexBuffer()
{
	Core::Function_Enter("D3DVertexBuffer::Destructor()");
	Release();
	Core::Function_Exit();
}

RESULT D3DVertexBuffer::Create(IDevice* Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF)
// creates a vertex buffer

{
	// save parameters
	Core::Function_Enter("D3DVertexBuffer::Create()");
	myFVF = FVF;
	myVertSize = VertSize;
	mySize = numVerts;
	myUsage = Usage;
	myFVF = FVF;
	// new buffer
	if(!myData)
		myData = new BYTE[numVerts*VertSize];
	myDev = Device;
	// create the buffer
	myDev->RemoveFromResetList(this);
	try
	{
		Device->CreateVertexBuffer(this, numVerts, VertSize, Usage, FVF,  (VOID**)&myD3DVB);
		if(myD3DVB == NULL)
			throw OUTOFMEMORY;
	}
	catch(RESULT cr)
	{
		Core::Function_Exit();
		return cr;
	}
	Core::Function_Exit();
	return OK;
	
}

RESULT D3DVertexBuffer::Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)
{
	// lock the buffer
	Core::Function_Enter("D3DVertexBuffer::Lock()");
	RESULT r;
	++myLockCount;
	if(myD3DVB == NULL)
	{
		--myLockCount;
		Core::Function_Exit();
		return VBNOTCREATED;
	}
	r = HRtoR(myD3DVB->Lock(OffsetToLock, SizeToLock, ptr, Flags));
	savedP = *ptr;
	Core::Function_Exit();
	return r;
}

RESULT D3DVertexBuffer::Unlock()
{
	// unlock the buffer
	Core::Function_Enter("D3DVertexBuffer::Unlock()");
	HRESULT hr;  
	if(myD3DVB == NULL)
	{
		Core::Function_Exit();
		return VBNOTCREATED;
	}
	memcpy(myData, savedP, mySize*myVertSize); // and save the data
	hr = myD3DVB->Unlock();
	if(SUCCEEDED(hr))
		--myLockCount;
	else
	{
		Core::Function_Exit();
		return HRtoR(hr);
	}
	
	Core::Function_Exit();
	return HRtoR(hr);
}

void D3DVertexBuffer::Release()
{
	// kill the buffer
	Core::Function_Enter("D3DVertexBuffer::Release()");
	if(myD3DVB != NULL) myD3DVB->Release();
	myD3DVB = NULL;
	if(myData) delete myData;
	myData = NULL;
	if(myDev)
		myDev->RemoveFromResetList(this);
	Core::Function_Exit();
	
}

RESULT D3DVertexBuffer::Set()
{
	// set this buffer to render from
	Core::Function_Enter("D3DVertexBuffer::Set()");
	if(myD3DVB == NULL)
	{
		Core::Function_Exit();
		return VBNOTCREATED;
	}
	myDev->SetVertexShader(myFVF);
	myDev->SetVertexBuffer(myD3DVB, myVertSize);
	Core::Function_Exit();
	return OK;
}

VOID D3DVertexBuffer::ResetDeviceObjects()
{
	// recreate this buffer
	Core::Function_Enter("D3DVertexBuffer::ResetDeviceObjects()");
	
	Create(myDev, mySize, myVertSize, myUsage, myFVF);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*myVertSize);
	Unlock();
	Core::Function_Exit();
	return;
}

void D3DVertexBuffer::Release_SaveData()
{
	if(myD3DVB != NULL) myD3DVB->Release();
	myD3DVB = NULL;
}