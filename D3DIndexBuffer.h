#ifndef _D3DINDEXBUFFER_H
#define _D3DINDEXBUFFER_H

#include <d3d8.h>
#include "IIndexBuffer.h"

class D3DDevice;

class D3DIndexBuffer : public IIndexBuffer
{
	friend D3DDevice;
public:
	D3DIndexBuffer(const char* id);
	virtual ~D3DIndexBuffer();

	//RESULT Create(IDevice* Device, UINT numIndices, DWORD usage, BYTE type);
	RESULT Lock(UINT offsetToLock, UINT sizeToLock, BYTE** ptr, DWORD flags);
	RESULT Unlock();
	RESULT Set();
	void Release();
	void Release_SaveData();
	VOID ResetDeviceObjects();

private:
	LPDIRECT3DINDEXBUFFER8 myD3DIB;
	D3DIndexBuffer();
};


#endif