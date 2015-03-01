#ifndef _D3DVERTEXBUFFER_H
#define _D3DVERTEXBUFFER_H

// just check over each of these functions, to make sure they work as desired.

#include "..\Common\CObject.h"
#include "..\Common\Result.h"
#include "..\DivGraph\IVertexBuffer.h"
#include "D3DDevice.h"


class _API D3DVertexBuffer : public IVertexBuffer
{
public:
	D3DVertexBuffer(char* id);
	virtual ~D3DVertexBuffer();
	
	RESULT Create(IDevice* Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF);
	
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

		
