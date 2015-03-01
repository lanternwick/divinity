#ifndef _ITEXTURE_H
#define _ITEXTURE_H
#include <assert.h>

#include "..\Common\IObject.h"
#include "..\Common\result.h"
#include "D3DDevice.h"

class ITexture : public IObject
{
public:
	
	ITexture(char* id){ }
	virtual ~ITexture(){}

	virtual RESULT LoadTexture(CSmartPtr<IDevice> Device, char* Filename)=0;
	virtual RESULT LoadTextureEx(CSmartPtr<IDevice> Device, char* filename, DWORD transColor)=0;
	virtual RESULT Set(CSmartPtr<IDevice> Device)=0;
	static RESULT MakeNull(CSmartPtr<IDevice> Device);
	virtual VOID ResetDeviceObjects()=0;
	virtual VOID Release()=0;
protected:
	char* myFilename;
	CSmartPtr<IDevice> myDev;
	BOOL Ex;
	DWORD myTransColor;
	ITexture(){}
};

_EXTERN_ template class _API CSmartPtr<ITexture>;

#endif