#ifndef _WORLDOBJ_H
#define _WORLDOBJ_H

#include "..\Common\CObject.h"
#include "..\Common\vector.h"
#include "..\Common\matrix.h"

class _API WorldObj : public CObject
{
public:
	WorldObj();
	virtual ~WorldObj();
	virtual BOOL SphereProbe(VECTOR* o, VECTOR* d);
	VOID SetPos(VECTOR* pos);
	VECTOR GetPos();
	VOID SetSphereRadius(float newRad);
	float GetSphereRadius();

	virtual RESULT Initialize()=0;
	virtual RESULT Render()=0;
	virtual RESULT Update()=0;

protected:
	float myRadius;
	VECTOR bbUpperLeftFront;
	VECTOR bbLowerRightRear;
	VECTOR myPos;
	MATRIX myTransform;
};


#endif