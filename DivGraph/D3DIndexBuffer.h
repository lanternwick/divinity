#ifndef _D3DINDEXBUFFER_H
#define _D3DINDEXBUFFER_H

#include "..\Common\CObject.h"
#include "IIndexBuffer.h"
#include "D3DDevice.h"

class D3DIndexBuffer : public IIndexBuffer
{
public:
	D3DIndexBuffer(char* id);
	~D3DIndexBuffer();

	RESULT Create(CSmartPtr<IDevice> Device, UINT numIndices, DWORD usage, BYTE type);
	RESULT Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags);
	RESULT Unlock();
	RESULT Set(CSmartPtr<IDevice> Device);
	void Release();
	VOID ResetDeviceObjects();

private:
	LPDIRECT3DINDEXBUFFER8 myD3DIB;
	D3DIndexBuffer();
};


#endif