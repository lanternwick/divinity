#ifndef _CINDEXBUFFER_H
#define _CINDEXBUFFER_H

class CIndexBuffer : public CObject
{
public:
	CIndexBuffer();
	~CIndexBuffer();

	RESULT Create(SYSDevice* Device, UINT size, DWORD usage);
	RESULT Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags);
	RESULT Unlock();
	RESULT Set(SYSDevice* Device);
	RESULT Release();

private:
	LPDIRECT3DINDEXBUFFER8 myD3DIB;
	UINT mySize;
	UINT myLockCount;
};

CIndexBuffer::CIndexBuffer()
{
	myD3DIB = NULL;
	myLockCount = 0;
	type = OBJ_INDEXBUFFER;
}

CIndexBuffer::~CIndexBuffer()
{
	Release();
}

RESULT CIndexBuffer::Create(SYSDevice* Device, UINT size, DWORD usage)
{
	Device->GetD3DDevice()->Device()->CreateIndexBuffer(size, usage, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &myD3DIB);

	return OK;
}

RESULT CIndexBuffer::Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags)
{
	myD3DIB->Lock(offsetToLock, sizeToLock, ptr, flags);
	++myLockCount;
	return OK;
}

RESULT CIndexBuffer::Unlock()
{
	myD3DIB->Unlock();
	--myLockCount;
	
	return OK;
}

RESULT CIndexBuffer::Set(SYSDevice* Device)
{
	Device->GetD3DDevice()->Device()->SetIndices(myD3DIB, 0);
	return OK;
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

#endif