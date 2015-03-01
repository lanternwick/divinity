#ifndef _D3DVERTEXBUFFER_H
#define _D3DVERTEXBUFFER_H

// just check over each of these functions, to make sure they work as desired.

#include "..\Common\CObject.h"
#include "..\Common\Result.h"
#include "IVertexBuffer.h"
#include "D3DDevice.h"


class _API D3DVertexBuffer : public IVertexBuffer
{
public:
	D3DVertexBuffer(char* id);
	~D3DVertexBuffer();
	
	RESULT Create(CSmartPtr<IDevice> Device, UINT numVerts, UINT VertSize, DWORD Usage, DWORD FVF);
	
	RESULT Lock(INT OffsetToLock, INT SizeToLock, BYTE** ptr, DWORD Flags);
	RESULT Unlock();
	RESULT Set(CSmartPtr<IDevice> Device);
	void Release();
	VOID ResetDeviceObjects();
protected:
	LPDIRECT3DVERTEXBUFFER8 myD3DVB;
	D3DVertexBuffer();
};

#endif

		
