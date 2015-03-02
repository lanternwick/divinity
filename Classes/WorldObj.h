#ifndef _WORLDOBJ_H
#define _WORLDOBJ_H

#include <CObject.h>

class WRLDObject : public CObject
{
public:
	WRLDObject();
	virtual ~WRLDObject();
	virtual BOOL SphereProbe(VECTOR* o, VECTOR* d);
	VOID SetPos(VECTOR* pos);
	VECTOR GetPos();
	VOID SetMesh(HANDLE newMesh);
	HANDLE GetMesh();
	VOID SetSphereRadius(float newRad);
	float GetSphereRadius();

	RESULT Render();
	RESULT Update();

protected:
	float myRadius;
	VECTOR bbUpperLeftFront;
	VECTOR bbLowerRightRear;
	VECTOR myPos;
	HANDLE myTransform;
	HANDLE myMesh;
};

WRLDObject::WRLDObject()
{
	myTransform = CreateMatrix();
}

WRLDObject::~WRLDObject()
{

}

BOOL WRLDObject::SphereProbe(VECTOR* o, VECTOR* d)
{

	return FALSE;
}

VOID WRLDObject::SetPos(VECTOR* pos)
{
	myPos = *pos;
	return;
}

VECTOR WRLDObject::GetPos()
{
	return myPos;
}

VOID WRLDObject::SetMesh(HANDLE newMesh)
{
	myMesh = newMesh;
	return;
}
	
HANDLE WRLDObject::GetMesh()
{
	return myMesh;
}

VOID WRLDObject::SetSphereRadius(float newRad)
{
	myRadius = newRad;
	return;
}

float WRLDObject::GetSphereRadius()
{
	return myRadius;
}

RESULT WRLDObject::Render()
{
	
	MatrixTranslation(myTransform, myPos.x, myPos.y, myPos.z);
	//MatrixIdentity(myTransform);

	SetMatrix(T_WORLD, myTransform);
	return RenderObject(myMesh);
}

RESULT WRLDObject::Update()
{

	return OK;
}

#endif