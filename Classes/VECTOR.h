#ifndef _VECTOR_H
#define _VECTOR_H


#define EXPORT __declspec(dllexport)

#include <math.h>

struct EXPORT VECTOR
{
	VECTOR();
	VECTOR(float X, float Y, float Z);

	float x;
	float y;
	float z;

	VECTOR operator + (VECTOR& r);
	VECTOR operator - (VECTOR& r);
	VECTOR operator / (float r);
	VECTOR operator * (float r);
	VECTOR& operator += (VECTOR& l);
	VECTOR& operator -= (VECTOR& l);
	VECTOR& operator /= (VECTOR& l);

};

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

inline float VectorMagnitude(VECTOR* a)
{
	return (float)sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

inline float DotProduct(VECTOR* a, VECTOR* b)
{
	return (a->x*b->x + a->y*b->y + a->z*b->z);
}

inline void VectorNormalize(VECTOR* r, VECTOR* a)
{
	float mag = VectorMagnitude(a);
	r->x = a->x / mag;
	r->y = a->y / mag;
	r->z = a->z / mag;
	return;
}

inline void VectorLerp(VECTOR* r, VECTOR* a, VECTOR* b, float s, float scaler)
{
	r->x = a->x + s * (b->x - a->x);
	r->y = a->y + s * (b->y - a->y);
	r->z = a->z + s * (b->z - a->z);

	VectorNormalize(r, r);

	*r = *r * scaler;
	return;
}

#endif
