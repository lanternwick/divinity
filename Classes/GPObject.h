#ifndef _GPOBJECT_H
#define _GPOBJECT_H



#include "CObject.h"

enum ACTOR_STATES
{
	ST_STILL = 0,
	ST_MOVING,
	ST_MOVING_WP,
	ST_ATTACKING,
	ST_ROTATING,
	ST_STARTWP,
};

class GPObject : public CObject
{
public:
	GPObject();
	virtual ~GPObject();
	GPObject* Next;
	GPObject* Prev;
	GPObject* First;
	virtual RESULT Render();
	virtual RESULT Update();
	VOID Setup();
	VOID SetLifeTime(DWORD t);
protected:
	DWORD myLifeTime;
	DWORD myBirth;
};

GPObject::GPObject()
{
	Next = NULL;
	Prev = NULL;
	//Type = OBJ_UNKNOWN;
}

GPObject::~GPObject()
{
}

VOID GPObject::Render() 
{	
	return;
}

VOID GPObject::Setup()
{
	Next = NULL;
	Prev = NULL;
}

VOID GPObject::Update()
{
#ifdef _SYSTEM_H
	VOID* dat;
	DWORD time;
	g_System->GetSystemData(SD_GAMETIME, dat);
	time = (DWORD)dat;
	if(time > myBirth + myLifeTime)
	{
		g_System->Delete(me);
	}
#endif
	return;
}

VOID GPObject::SetLifeTime(DWORD t)
{
	myLifeTime = t;
	return;
}

#endif