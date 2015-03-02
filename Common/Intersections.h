#ifndef _INTERSECTIONS_H
#define _INTERSECTIONS_H

#include "define_exports.h"
#include "plane.h"
#include "vector.h"

_API bool RayIntersectPlane(VECTOR* r, PLANE* p, VECTOR* a, VECTOR* d);
_API bool LineIntersectPlane(VECTOR* r, PLANE* p, VECTOR* start, VECTOR* end);
_API bool RayIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* a, VECTOR* d);
_API bool LineIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* start, VECTOR* end);

#endif