#ifndef _VECTOR_H
#define _VECTOR_H

#include <math.h>

struct VECTOR
{
	float x;
	float y;
	float z;

	VECTOR();
	VECTOR(float X, float Y, float Z);

	VECTOR operator+ (VECTOR& r);
	VECTOR operator- (VECTOR& r);
	VECTOR& operator+= (VECTOR& l);
	VECTOR& operator-= (VECTOR& l);
	VECTOR operator/ (float r);
	VECTOR operator* (float r);
		
	VECTOR& operator/= (VECTOR& l);
};

VECTOR operator / (float lhs, VECTOR rhs);

bool VertexInTriangle(VECTOR* vtxPoint, VECTOR* vtx0, VECTOR* vtx1, VECTOR* vtx2);

inline float VectorMagnitude(VECTOR* a)
{
	return (float)sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

inline float DotProduct(VECTOR* a, VECTOR* b)
{
	return (a->x*b->x + a->y*b->y + a->z*b->z);
}

inline void VectorCross(VECTOR* r, VECTOR* a, VECTOR* b)
{
	r->x = ((a->y * b->z) - (a->z * b->y));
	r->y = ((a->z * b->x) - (a->x * b->z));
	r->z = ((a->x * b->y) - (a->y * b->x));

	return;
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