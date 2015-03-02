#include <cobject.h>

CObject::CObject()
{
	type = OBJ_UNKNOWN;
	allowsMsgs = FALSE;
	FirstChild = FALSE;
	Sibling = NULL;
	Parent = NULL;
	Name = new char[128];
	strcpy(Name, "[No Name]");
	deleteFlag = FALSE;
}

CObject::~CObject()
{

}


OBJ_TYPES CObject::QueryObject()
{
	return type;
}

RESULT CObject::Render(DWORD& numPrims)
{

	return OK;
}

RESULT CObject::Update()
{

	DWORD tickC = GetTicks();
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

DMSG CObject::PeekMessasge()
{
	if(allowsMsgs)
		return MsgQueue.peek();
	DMSG b = {0, 0, 0, 0};
	return b;
}

VOID CObject::SendMessage(DMSG& message)
{
	if(allowsMsgs)
		MsgQueue.push(message);
	return;
}

VOID CObject::AllowMessages(BOOL s)
{
	allowsMsgs = s;
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