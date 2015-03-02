
#include "vector.h"
#include <windows.h>
#include <math.h>


VECTOR::VECTOR()
{
	ZeroMemory(&x, sizeof(float)*3);
}

VECTOR::VECTOR(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}

VECTOR VECTOR::operator+ (VECTOR& r)
{
	VECTOR ret;
	ret.x = x + r.x;
	ret.y = y + r.y;
	ret.z = z + r.z;
	return ret;
}

VECTOR VECTOR::operator- (VECTOR& r)
{
	VECTOR ret;
	ret.x = x - r.x;
	ret.y = y - r.y;
	ret.z = z - r.z;
	return ret;
}

VECTOR& VECTOR::operator+= (VECTOR& l)
{	
	x += l.x;
	y += l.y;
	z += l.z;
	return *this;
}

VECTOR& VECTOR::operator-= (VECTOR& l)
{	
	x -= l.x;
	y -= l.y;
	z -= l.z;
	return *this;
}

VECTOR VECTOR::operator/ (float r)
{	
	VECTOR ret;
	ret.x = x / r;
	ret.y = y / r;
	ret.z = z / r;
	return ret;
}

VECTOR VECTOR::operator* (float r)
{	
	VECTOR ret;
	ret.x = x * r;
	ret.y = y * r;
	ret.z = z * r;
	return ret;
}

VECTOR& VECTOR::operator/= (VECTOR& l)
{	
	x /= l.x;
	y /= l.y;
	z /= l.z;
	return *this;
}

VECTOR operator / (float lhs, VECTOR rhs)
{
	VECTOR ret;
	ret = rhs;
	if(ret.x != 0.0f)
		ret.x = lhs / ret.x;
	if(ret.y != 0.0f)
		ret.y = lhs / ret.y;
	if(ret.z != 0.0f)
		ret.z = lhs / ret.z;
	return ret;
}

bool VertexInTriangle(VECTOR* vtxPoint, VECTOR* vtx0, VECTOR* vtx1, VECTOR* vtx2)
{
    double dAngle;

    VECTOR vec0;
    VECTOR vec1;
    VECTOR vec2;
	//if(vtx0.x - INT(vtx0.x) < 0.01)
	VectorNormalize(&vec0, &(*vtxPoint - *vtx0));
	VectorNormalize(&vec1, &(*vtxPoint - *vtx1));
	VectorNormalize(&vec2, &(*vtxPoint - *vtx2));
    dAngle =
        acos(DotProduct(&vec0, &vec1)) + 
        acos(DotProduct(&vec1, &vec2)) + 
        acos(DotProduct(&vec2, &vec0));


    if(fabs(dAngle - 2*3.14159) < 0.1f)
        return true;
    else
        return false;
}