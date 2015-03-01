#include "CVertexBuffer.h"

CVertexBuffer::CVertexBuffer()
{
	type = OBJ_VERTEXBUFFER;
	myLockCount = 0;
	myD3DVB = NULL;
	myOGLVB = NULL;
}

CVertexBuffer::~CVertexBuffer()
{
	Release();
}

RESULT CVertexBuffer::Create(SYSDevice* Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF)
// creates a vertex buffer

{
	sel = Device->GetAPI();
	myFVF = FVF;
	myVertSize = VertSize;
	mySize = numVerts;
	myUsage = Usage;
	myFVF = FVF;
	HRESULT hr;
	myData = new BYTE[numVerts*VertSize];
	myDev = Device;
	switch(sel)
	{
	case API_D3D:
		hr = Device->GetD3DDevice()->Device()->CreateVertexBuffer(numVerts*VertSize, Usage, FVF, D3DPOOL_DEFAULT, &myD3DVB);
		if(myD3DVB == NULL)
			return OUTOFMEMORY;
		return OK;
	case API_OGL:
		return NOTIMPL;
		myOGLVB = new BYTE[numVerts*VertSize];
		if(myOGLVB == NULL)
			return OUTOFMEMORY;
		return OK;
	default:
		return APINONE;
	}
}

RESULT CVertexBuffer::Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags)
{
	RESULT r;
	switch(sel)
	{
	case API_D3D:
		++myLockCount;
		if(myD3DVB == NULL)
		{
			--myLockCount;
			return VBNOTCREATED;
		}
		r = HRtoR(myD3DVB->Lock(OffsetToLock, SizeToLock, ptr, Flags));
		savedP = *ptr;
		return r;
	case API_OGL:
		return NOTIMPL;
		++myLockCount;
		if(myOGLVB == NULL)
		{
			--myLockCount;
			return VBNOTCREATED;
		}
		ptr = &myOGLVB+OffsetToLock;
		savedP = *ptr;
		return OK;
	default:
		return APINONE;
	}
}

RESULT CVertexBuffer::Unlock()
{
	HRESULT hr;
	switch(sel)
	{
	case API_D3D:
		if(myD3DVB == NULL)
			return VBNOTCREATED;
		hr = myD3DVB->Unlock();
		if(SUCCEEDED(hr))
			--myLockCount;
		else
			return HRtoR(hr);
		break;
	case API_OGL:
		return NOTIMPL;
		if(myOGLVB == NULL)
			return VBNOTCREATED;
		--myLockCount;
		break;
	default:
		return APINONE;
	}
	memcpy(myData, savedP, mySize);
	return OK;
}

RESULT CVertexBuffer::Release()
{
	if(myD3DVB != NULL) myD3DVB->Release();
	if(myOGLVB != NULL) delete[] myOGLVB;
	return OK;
}

RESULT CVertexBuffer::Set(SYSDevice* Device)
{
	switch(sel)
	{
	case API_D3D:
		if(myD3DVB == NULL)
			return VBNOTCREATED;
		Device->GetD3DDevice()->Device()->SetVertexShader(myFVF);
		Device->GetD3DDevice()->Device()->SetStreamSource(0, myD3DVB, myVertSize);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}
}

VOID CVertexBuffer::ResetDeviceObjects()
{
	Release();
	Create(myDev, mySize, myVertSize, myUsage, myFVF);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*myVertSize);
	Unlock();
	return;
}