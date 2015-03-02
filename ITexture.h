#ifndef _ITEXTURE_H
#define _ITEXTURE_H
#include <assert.h>

#include "IObject.h"
#include "result.h"
#include "IMediaObject.h"

class IDevice;

class ITexture : public IMediaObject
{
public:
	
	ITexture(const char* id){ m_id = id; }
	virtual ~ITexture(){}

	//virtual RESULT LoadTexture(IDevice* Device, char* Filename)=0;
	//virtual RESULT CreateTextureFromMemory(IDevice* device, char* memory, int width)=0;
	//virtual RESULT LoadTextureEx(IDevice* Device, char* filename, DWORD transColor)=0;
	virtual RESULT Set(unsigned long stage)=0;
	static RESULT MakeNull(IDevice* Device, unsigned long stage);
	virtual VOID ResetDeviceObjects()=0;
	virtual VOID Release()=0;
	int DecRef();
	int IncRef();
	long Width();
protected:
	char* myFilename;
	IDevice* myDev;
	BOOL Ex;
	DWORD myTransColor;
	long m_width;
	long m_refc;
	ITexture(){}
};



#endif