
#include "..\Common\WorldObj.h"


WorldObj::WorldObj()
{
	
}

WorldObj::~WorldObj()
{

}

BOOL WorldObj::SphereProbe(VECTOR* o, VECTOR* d)
{

	return FALSE;
}

VOID WorldObj::SetPos(VECTOR* pos)
{
	// set position
	myPos = *pos;
	return;
}

VECTOR WorldObj::GetPos()
{
	// get position
	return myPos;
}

VOID WorldObj::SetSphereRadius(float newRad)
{
	// set sphere radius
	myRadius = newRad;
	return;
}

float WorldObj::GetSphereRadius()
{
	// return sphere radius
	return myRadius;
}
