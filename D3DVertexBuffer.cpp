#include "D3DVertexBuffer.h"
#include "IDevice.h"

D3DVertexBuffer::D3DVertexBuffer(const char* id)
{
	myLockCount = 0;
	myD3DVB = NULL;
	myData = NULL;
	myDev = NULL;
	if(id)
		m_id() = id;
}

D3DVertexBuffer::D3DVertexBuffer()
{
	myLockCount = 0;
	myD3DVB = NULL;
	myData = NULL;
	myDev = NULL;
}

D3DVertexBuffer::~D3DVertexBuffer()
{
	Release();
}

/*RESULT D3DVertexBuffer::Create(IDevice* Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF)
// creates a vertex buffer

{
	// save parameters
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
		return cr;
	}
	return OK;
	
}*/

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
	{
		return VBNOTCREATED;
	}
	memcpy(myData, savedP, mySize*myVertSize); // and save the data
	hr = myD3DVB->Unlock();
	if(SUCCEEDED(hr))
		--myLockCount;
	else
	{
		return HRtoR(hr);
	}
	
	return HRtoR(hr);
}

void D3DVertexBuffer::Release()
{
	// kill the buffer
	if(myD3DVB != NULL) 
		myD3DVB->Release();
	myD3DVB = NULL;
	if(myData) 
		delete myData;
	myData = NULL;
	if(myDev)
		myDev->RemoveFromResetList(this);
	
}

RESULT D3DVertexBuffer::Set()
{
	// set this buffer to render from
	if(myD3DVB == NULL)
	{
		return VBNOTCREATED;
	}
	myDev->SetVertexShader(myFVF);
	myDev->SetVertexBuffer(myD3DVB, myVertSize);
	return OK;
}

VOID D3DVertexBuffer::ResetDeviceObjects()
{
	// recreate this buffer
	IVertexBuffer* t = this;
	myDev->CreateVertexBuffer(mySize, myVertSize, myUsage, myFVF, t, NULL);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*myVertSize);
	Unlock();
	return;
}

void D3DVertexBuffer::Release_SaveData()
{
	if(myD3DVB != NULL) myD3DVB->Release();
	myD3DVB = NULL;
}