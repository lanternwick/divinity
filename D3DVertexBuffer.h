#ifndef _D3DVERTEXBUFFER_H
#define _D3DVERTEXBUFFER_H

// just check over each of these functions, to make sure they work as desired.

#include <d3d8.h>

#include "Result.h"
#include "IVertexBuffer.h"

class D3DDevice;

class D3DVertexBuffer : public IVertexBuffer
{
	friend D3DDevice;
public:
	D3DVertexBuffer(const char* id);
	virtual ~D3DVertexBuffer();
	
	//RESULT Create(IDevice* Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF);
	
	RESULT Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags);
	RESULT Unlock();
	RESULT Set();
	void Release();
	VOID ResetDeviceObjects();
	void Release_SaveData();
protected:
	LPDIRECT3DVERTEXBUFFER8 myD3DVB;
	D3DVertexBuffer();
};

#endif

		
