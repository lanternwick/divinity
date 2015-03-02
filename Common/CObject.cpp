#include "..\Common\cobject.h"

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

CObject::CObject()
{
	type = OBJ_UNKNOWN;
	Name = new char[128];
	strcpy(Name, "[No Name]");
	deleteFlag = FALSE;
	Sibling = NULL;
	Parent = NULL;
	FirstChild = NULL;
}

CObject::~CObject()
{
	delete Name;
}


OBJ_TYPES CObject::QueryObject()
{
	return type;
}

RESULT CObject::Render()
{

	return OK;
}

RESULT CObject::Update()
{

	DWORD tickC = Core::Instance()->GetTickCount();
	if(tickC >= deathTick)
	{
		deleteFlag = TRUE | HasTimeout;
	}
	return OK;
}

VOID CObject::Activate()
{
	active = TRUE;
}

VOID CObject::Deactivate()
{
	active = FALSE;
}

BOOL CObject::ShouldDelete()
{
	return deleteFlag;
}

VOID CObject::SetTimeout(DWORD newDeathTick)
{
	HasTimeout = TRUE;
	deathTick = newDeathTick;
}

VOID CObject::ResetDeviceObjects()
{

	return;
}