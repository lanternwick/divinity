
#include "..\Common\vector.h"
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

