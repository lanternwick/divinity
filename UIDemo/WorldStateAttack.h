#ifndef _WRLDSTATEATTACK_H
#define _WRLDSTATEATTACK_H

class WRLDStateAttack : public WRLDState
{
public:
	WRLDStateAttack();
	~WRLDStateAttack();

	RESULT OnEnter(WRLDState* lastState);
	RESULT OnExit(WRLDState* nextState);
	RESULT Update();

protected:
	WRLDActor* myTarget;
};

WRLDStateMove::WRLDStateMove()
{
	
}

WRLDStateMove::~WRLDStateMove()
{

}

RESULT WRLDStateMove::OnEnter(WRLDState* lastState)
{

	return OK;
}

RESULT WRLDStateMove::OnExit(WRLDState* nextState)
{

	return OK;
}

RESULT WRLDStateMove::Update()
{
	
	return OK;
}

#endif
