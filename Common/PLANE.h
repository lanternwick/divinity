#ifndef _PLANE_H
#define _PLANE_H

#include "vector.h"

struct _API PLANE {
	float a;
	float b;
	float c;
	float d;
};

_API PLANE* PlaneFromPoints(PLANE* p, VECTOR* v1, VECTOR* v2, VECTOR* v3);

#endif