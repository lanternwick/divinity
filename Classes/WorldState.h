#ifndef _WRLDSTATE_H
#define _WRLDSTATE_H

class WRLDActor;

class WRLDState
{
public:
	WRLDState();
	~WRLDState();

	virtual RESULT Update()=0;
	virtual RESULT OnEnter(WRLDState* lastState)=0;
	virtual RESULT OnExit(WRLDState* nextState)=0;
	
	VOID SetActor(WRLDActor* newActor);
	BOOL Complete();
protected:
	WRLDActor* myActor;
	BOOL complete;
};

WRLDState::WRLDState()
{
	complete = FALSE;
	myActor = NULL;
}

WRLDState::~WRLDState()
{

}

VOID WRLDState::SetActor(WRLDActor* newActor)
{
	myActor = newActor;
	return;
}

BOOL WRLDState::Complete()
{
	return complete;
}

#endif

