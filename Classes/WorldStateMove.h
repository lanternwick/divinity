#ifndef _WRLDSTATEMOVE_H
#define _WRLDSTATEMOVE_H

class WRLDStateMove : public WRLDState
{
public:
	WRLDStateMove();
	~WRLDStateMove();

	RESULT OnEnter(WRLDState* lastState);
	RESULT OnExit(WRLDState* nextState);
	RESULT Update();

protected:
	float myLerpVal;
	float myMoveLerpVal;
	float myDist;
	VECTOR myDest;
};

WRLDStateMove::WRLDStateMove()
{
	myLerpVal = 0.0f;
	myMoveLerpVal = 0.0f;
	complete = FALSE;
}

WRLDStateMove::~WRLDStateMove()
{

}

RESULT WRLDStateMove::OnEnter(WRLDState* lastState)
{
	VECTOR dist = myActor->GetDest() - myActor->GetPos();
	myActor->SetStartPos(&myActor->GetPos());
	myDist = VectorMagnitude(&dist);
	myDest = myActor->GetDest();
	return OK;
}

RESULT WRLDStateMove::OnExit(WRLDState* nextState)
{

	return OK;
}

RESULT WRLDStateMove::Update()
{
	VECTOR currVel = myActor->GetVelocity();
	VECTOR currPos = myActor->GetPos();
	VECTOR currDest = myDest;
	VECTOR destVel = currDest - currPos;
	VECTOR startPos = myActor->GetStartPos();
	VECTOR rVel;
	VECTOR rPos;
	float currSpeed = myActor->GetSpeed();

	if(Get2DDist(currPos.x, currPos.z, currDest.x, currDest.z) < 0.5f)
	{
		complete = TRUE;
		return OK;
	}

	currVel = currDest - currPos;
	VectorNormalize(&currVel, & currVel);
	rPos.x = currPos.x + (currVel.x) * TimeDelta() * currSpeed;
	rPos.y = currPos.y + (currVel.y) * TimeDelta() * currSpeed;
	rPos.z = currPos.z + (currVel.z) * TimeDelta() * currSpeed;

	myActor->SetPos(&rPos);
	
	return OK;
}

#endif
