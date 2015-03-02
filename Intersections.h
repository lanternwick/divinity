#ifndef _INTERSECTIONS_H
#define _INTERSECTIONS_H

#include "plane.h"
#include "vector.h"

bool RayIntersectPlane(VECTOR* r, PLANE* p, VECTOR* a, VECTOR* d);
bool LineIntersectPlane(VECTOR* r, PLANE* p, VECTOR* start, VECTOR* end);
bool RayIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* a, VECTOR* d);
bool LineIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* start, VECTOR* end);

#endif