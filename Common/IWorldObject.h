#ifndef _IWORLDOBJECT_H
#define _IWORLDOBJECT_H

#include "..\Common\IObject.h"
#include "..\Common\vector.h"
#include "..\Common\matrix.h"

class _API IWorldObject : public IObject
{
public:
	IWorldObject();
	virtual ~WorldObject();
	virtual BOOL SphereProbe(VECTOR* o, VECTOR* d);
	VOID SetPos(VECTOR* pos);
	VECTOR& GetPos() const;
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