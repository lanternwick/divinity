#ifndef _RPTVECTOR_H
#define _RPTVECTOR_H

#include <VECTOR.h>
#include <result.h>

class RPTVECTOR
{
public:
	float r;
	float p;
	float t;

};

VOID RPTtoXYZ(VECTOR* result, RPTVECTOR* s)
{
	result->x = float(cos(s->t) * cos(s->p));
	result->z = float(cos(s->t) * sin(s->p));
	result->y = float(sin(s->t));
	result->x *= s->r;
	result->y *= s->r;
	result->z *= s->r;
	return;
}

#endif