#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "plane.h"


class ray
{
public:
	VECTOR orig;
	VECTOR dir;

	bool intersect_sphere(VECTOR* r, VECTOR* s_c, float s_r, float* r_t=0);
	bool intersect_circle(VECTOR* r, VECTOR* s_c, float s_r, float* r_t=0);
	bool intersect_plane(VECTOR* r, PLANE* pl);
};

#endif