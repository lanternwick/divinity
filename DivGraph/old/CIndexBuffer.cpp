#include "CIndexBuffer.h"


CIndexBuffer::CIndexBuffer()
{
	myD3DIB = NULL;
	myLockCount = 0;
	type = OBJ_INDEXBUFFER;
	sel = API_NONE;
}

CIndexBuffer::~CIndexBuffer()
{
	Release();
}

RESULT CIndexBuffer::Create(SYSDevice* Device, UINT numIndices, DWORD usage)
{
	sel = Device->GetAPI();
	myDevice = Device;
	mySize = numIndices;
	myUsage = usage;
	switch(sel)
	{
	case API_D3D:
		Device->GetD3DDevice()->Device()->CreateIndexBuffer(numIndices*sizeof(WORD), usage, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &myD3DIB);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}

	return IMPOSSIBLE;
}

RESULT CIndexBuffer::Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)
{
	++myLockCount;
	switch(sel)
	{
	case API_D3D:
		myD3DIB->Lock(offsetToLock, sizeToLock, ptr, flags);
		return OK;
	case API_OGL:
		--myLockCount;
		return NOTIMPL;
	default:
		--myLockCount;
		return APINONE;
	}
	
	return IMPOSSIBLE;
}

RESULT CIndexBuffer::Unlock()
{
	--myLockCount;
	switch(sel)
	{
	case API_D3D:
		myD3DIB->Unlock();
		return OK;
	case API_OGL:
		++myLockCount;
		return NOTIMPL;
	default:
		++myLockCount;
		return APINONE;
	}
	
	return IMPOSSIBLE;
}

RESULT CIndexBuffer::Set(SYSDevice* Device)
{
	switch(sel)
	{
	case API_D3D:
		Device->GetD3DDevice()->Device()->SetIndices(myD3DIB, 0);
		return OK;
	case API_OGL:
		return NOTIMPL;
	default:
		return APINONE;
	}

	return IMPOSSIBLE;
}

RESULT CIndexBuffer::Release()
{
	if(myD3DIB != NULL)
	{
		myD3DIB->Release();
		myD3DIB = NULL;
	}
	return OK;
}

VOID CIndexBuffer::ResetDeviceObjects()
{
	Release();
	Create(myDevice, mySize, myUsage);
	BYTE* p;
	Lock(0, 0, &p, 0);
	memcpy(p, myData, mySize*sizeof(WORD));
	Unlock();
	return;
}