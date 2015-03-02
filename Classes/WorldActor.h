#ifndef _WORLDACTOR_H
#define _WORLDACTOR_H

#include <worldobj.h>
#include <queue>
#include <worldactor.h>
#include <worldstate.h>

class WRLDActor : public WRLDObject
{
public:
	WRLDActor();
	~WRLDActor();

	RESULT Update();
	RESULT Render();

	VOID AddState(WRLDState* state);
	VECTOR GetVelocity();
	VOID SetVelocity(VECTOR* newVel);
	VECTOR GetDest();
	VOID SetDest(VECTOR* newDest);
	float GetSpeed();
	VOID SetSpeed(float newSpeed);
	VECTOR GetStartPos();
	VOID SetStartPos(VECTOR* newStart);
	INT HP;
protected:
	VECTOR myVel;
	VECTOR myDest;
	VECTOR myStart;
	float mySpeed;
	std::queue<WRLDState*> StateQueue;
	WRLDState* mySetState;
	
};

WRLDActor::WRLDActor()
{
	mySetState = NULL;
	return;
}

WRLDActor::~WRLDActor()
{

	return;
}

RESULT WRLDActor::Update()
{
	WRLDState* lstate;
	CObject::Update();
	if(mySetState)
	{
		mySetState->Update();
		if(mySetState->Complete())
		{
			if(!StateQueue.empty())
			{
				lstate = StateQueue.front();
				mySetState->OnExit(lstate);
				lstate = mySetState;
				mySetState = StateQueue.front();
				StateQueue.pop();
				mySetState->SetActor(this);
				mySetState->OnEnter(lstate);
			}
			else
			{
					mySetState->OnExit(NULL);
			}
			delete mySetState;
			mySetState = NULL;
		}
	}
	else
	{
		if(!StateQueue.empty())
		{
			mySetState = StateQueue.front();
			StateQueue.pop();
			mySetState->SetActor(this);
			mySetState->OnEnter(NULL);
		}
	}
	
	return OK;
}

RESULT WRLDActor::Render()
{
	WRLDObject::Render();
	return OK;
}

VOID WRLDActor::AddState(WRLDState* state)
{
	StateQueue.push(state);
	return;
}

VECTOR WRLDActor::GetVelocity()
{
	return myVel;
}

VOID WRLDActor::SetVelocity(VECTOR* newVel)
{
	myVel = *newVel;
	return;
}

VECTOR WRLDActor::GetDest()
{
	return myDest;
}

VOID WRLDActor::SetDest(VECTOR* newDest)
{
	myDest = *newDest;
	return;
}

float WRLDActor::GetSpeed()
{
	return mySpeed;
}

VOID WRLDActor::SetSpeed(float newSpeed)
{
	mySpeed = newSpeed;
	return;
}

VECTOR WRLDActor::GetStartPos()
{
	return myStart;
}

VOID WRLDActor::SetStartPos(VECTOR* newStart)
{
	myStart = *newStart;
	return;
}

#endif
