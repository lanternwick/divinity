#ifndef _PLANE_H
#define _PLANE_H

#include "vector.h"

struct PLANE {
	float a;
	float b;
	float c;
	float d;
};

PLANE* PlaneFromPoints(PLANE* p, VECTOR* v1, VECTOR* v2, VECTOR* v3);

#endif