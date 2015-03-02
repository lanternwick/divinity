#ifndef _COBJECT_H
#define _COBJECT_H

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

VOID ObjText(OBJ_TYPES type, char** str)
{
	switch(type)
	{
	case OBJ_TEXTURE:
		strcpy(*str, "OBJ_TEXTURE");
		return;
	case OBJ_MESH:
		strcpy(*str, "OBJ_MESH");
		return;
	case OBJ_WINDOW:
		strcpy(*str, "OBJ_WINDOW");
		return;
	case OBJ_VERTEXBUFFER:
		strcpy(*str, "OBJ_VERTEXBUFFER");
		return;
	case OBJ_DFONT:
		strcpy(*str, "OBJ_DFONT");
		return;
	case OBJ_SEGMENT:
		strcpy(*str, "OBJ_SEGMENT");
		return;
	case OBJ_SCROLLBAR:
		strcpy(*str, "OBJ_SCROLLBAR");
		return;
	case OBJ_LABEL:
		strcpy(*str, "OBJ_LABEL");
		return;
	case OBJ_DISPLAYBOX:
		strcpy(*str, "OBJ_DISPLAYBOX");
		return;
	case OBJ_PARTICLESYSTEM:
		strcpy(*str, "OBJ_PARTICLESYSTEM");
		return;
	case OBJ_EMITTER:
		strcpy(*str, "OBJ_EMITTER");
		return;
	case OBJ_INDEXBUFFER:
		strcpy(*str, "OBJ_INDEXBUFFER");
		return;
	case OBJ_UNKNOWN:
		strcpy(*str, "OBJ_UNKNOWN");
		return;
	case OBJ_ANY:
		strcpy(*str, "OBJ_ANY");
		return;
	default:
		strcpy(*str, "uhhh...");
		return;
	}
	return;
}

struct SYSTEM_API DMSG
{
	HANDLE src;
	UINT Message;
	DWORD param1;
	DWORD param2;
	DWORD param3;
};



#include <queue2.h>
#include <result.h>

DWORD SYSTEM_API GetTicks();


class SYSTEM_API CObject
{
public:
	CObject();
	virtual ~CObject();
	char* Name;
	HANDLE me;
	OBJ_TYPES QueryObject();
	
	virtual RESULT Render(DWORD& numPrims);
	virtual RESULT Update();

	VOID Activate(); 
	VOID Deactivate();
	
	DMSG PeekMessasge();
	VOID SendMessage(DMSG& message);
	CObject* FirstChild;
	CObject* Sibling;
	CObject* Parent;
	VOID AllowMessages(BOOL s);
	DWORD extra;
	BOOL ShouldDelete();
	VOID SetTimeout(DWORD newDeathTick);

	virtual VOID ResetDeviceObjects();
protected:
	OBJ_TYPES type;
	BOOL active;
	Queue MsgQueue;
	BOOL allowsMsgs;
	MessageProc* myProc;
	BOOL deleteFlag;
	DWORD deathTick;
	BOOL HasTimeout;
	
};

#endif