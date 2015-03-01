#ifndef _COBJECT_H
#define _COBJECT_H

#include <windows.h>
#include "..\divcore\divcore.h"

enum OBJ_TYPES
{
	OBJ_TEXTURE = 1,
	OBJ_MESH = 2,
	OBJ_WINDOW = 4,
	OBJ_VERTEXBUFFER = 8, 
	OBJ_DFONT = 16, 
	OBJ_SEGMENT = 32,
	OBJ_SCROLLBAR = 64,
	OBJ_LABEL = 128,
	OBJ_DISPLAYBOX = 256,
	OBJ_PARTICLESYSTEM = 512,
	OBJ_EMITTER = 1024,
	OBJ_INDEXBUFFER = 2048,
	OBJ_UNKNOWN = 4096,

	OBJ_ANY,
};

VOID ObjText(OBJ_TYPES type, char** str);


struct __declspec(dllexport) DMSG
{
	HANDLE src;
	UINT Message;
	DWORD param1;
	DWORD param2;
	DWORD param3;
};




#include "..\Common\result.h"

class _API CObject
{
public:
	CObject();
	virtual ~CObject();
	char* Name;
	OBJ_TYPES QueryObject();
	
	virtual RESULT Render();
	virtual RESULT Update();

	VOID Activate(); 
	VOID Deactivate();
	
	CObject* FirstChild;
	CObject* Sibling;
	CObject* Parent;
	BOOL ShouldDelete();
	VOID SetTimeout(DWORD newDeathTick);

	virtual VOID ResetDeviceObjects();
protected:
	OBJ_TYPES type;
	BOOL active;
	BOOL deleteFlag;
	DWORD deathTick;
	BOOL HasTimeout;
	
};

#endif