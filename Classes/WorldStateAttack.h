#ifndef _WRLDSTATEATTACK_H
#define _WRLDSTATEATTACK_H

#include <WorldStateMove.h>

class WRLDStateAttack : public WRLDState
{
public:
	WRLDStateAttack();
	~WRLDStateAttack();

	RESULT OnEnter(WRLDState* lastState);
	RESULT OnExit(WRLDState* nextState);
	RESULT Update();
	WRLDActor* myTarget;
protected:
	
	WRLDStateMove* MState;
};

WRLDStateAttack::WRLDStateAttack()
{
	complete = FALSE;
	MState = NULL;
}

WRLDStateAttack::~WRLDStateAttack()
{

}

RESULT WRLDStateAttack::OnEnter(WRLDState* lastState)
{
	VECTOR myPos, TargPos;
	myPos = myActor->GetPos();
	TargPos = myTarget->GetPos();
	if(Get3DDist(myPos, TargPos) >= 0.5f)
	{
		MState = new WRLDStateMove;
		MState->SetActor(myActor);
		myActor->SetDest(&TargPos);
		MState->OnEnter(NULL);
	}
	return OK;
}

RESULT WRLDStateAttack::OnExit(WRLDState* nextState)
{

	return OK;
}

RESULT WRLDStateAttack::Update()
{
	DMSG m;
	if(MState)
	{
		if(MState->Complete())
		{
			MState->OnExit(NULL);
			delete MState;
			MState = NULL;
			OnEnter(NULL);
		}
		else
		{
			MState->Update();
		}
	}
	else
	{
		OnEnter(NULL);
		if(MState != NULL)
			return OK;
		m.Message = DM_BEING_ATTACKED;
		myTarget->SendMessage(m);
		myTarget->HP -= 10;
		m.Message = DM_DAMAGED;
		myTarget->SendMessage(m);
		if(myTarget->HP <= 0)
		{
			myTarget->SetTimeout(GetTicks()+100);
			complete = TRUE;
		}
	}
	return OK;
}

#endif
