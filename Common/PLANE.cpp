#include "plane.h"

_API PLANE* PlaneFromPoints(PLANE* p, VECTOR* v1, VECTOR* v2, VECTOR* v3)
{
	/* Algorithm:
	A = y1 (z2 - z3) + y2 (z3 - z1) + y3 (z1 - z2) 
	B = z1 (x2 - x3) + z2 (x3 - x1) + z3 (x1 - x2) 
	C = x1 (y2 - y3) + x2 (y3 - y1) + x3 (y1 - y2) 
	- D = x1 (y2 z3 - y3 z2) + x2 (y3 z1 - y1 z3) + x3 (y1 z2 - y2 z1)
	*/
	p->a = v1->y * (v2->z - v3->z) + v2->y * (v3->z - v1->z) + v3->y * (v1->z - v2->z);
	p->b = v1->z * (v2->x - v3->x) + v2->z * (v3->x - v1->x) + v3->z * (v1->x - v2->x);
	p->c = v1->x * (v2->y - v3->y) + v2->x * (v3->y - v1->y) + v3->x * (v1->y - v2->y);
	p->d = -1.0f * (v1->x * (v2->y * v3->z - v3->y * v2->z) + v2->x * (v3->y * v1->z - v1->y * v3->z)
					+ v3->x * (v1->y * v2->z - v2->y * v1->z));

	return p;
}