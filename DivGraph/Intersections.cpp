#include "..\Common\Intersections.h"

#define square(x) ((x)*(x))

_API bool RayIntersectPlane(VECTOR* r, PLANE* p, VECTOR* a, VECTOR* d)
{
	float cosAlpha = p->a * d->x + p->b * d->y + p->c * d->z;
	float dot2;
	float t;
	if(fabs(cosAlpha) < 0.01f)
		return false;
	dot2 = p->a * a->x + p->b * a->y + p->c * a->z;
	t = -1 * dot2 / cosAlpha;
	if(t < 0) return false;
	r->x = a->x + d->x * t;
	r->y = a->y + d->y * t;
	r->z = a->z + d->z * t;
	return true;
}

_API bool LineIntersectPlane(VECTOR* r, PLANE* p, VECTOR* start, VECTOR* end)
{
	VECTOR d;
	d.x = end->x - start->x;
	d.y = end->y - start->y;
	d.z = end->z - start->z;
	VectorNormalize(&d, &d);
	return RayIntersectPlane(r, p, start, &d);
}

_API bool RayIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* a, VECTOR* d)
{
	/*float ca;
	float cb;
	float cc;
	float discrim;
	float t0;
	float t1;

	VectorNormalize(d, d);
	ca = d->x * d->x + d->y * d->y + d->z * d->z;
	cb = 2 * (d->x * (a->x - sphereC->x) + d->y * (a->y - sphereC->y) + d->z * (a->z - sphereC->z));
	cc = square((a->x - sphereC->x)) + square((a->y - sphereC->y)) + square((a->z - sphereC->z)) - square(sphereR);
	
	discrim = square(cb) - 4 * cc;
	if(discrim < 0.0f)
		return false;
	
	t0 = (-1.0f * cb + float(sqrt(discrim)) / 2.0f);
	t1 = (-1.0f * cb - float(sqrt(discrim)) / 2.0f);

	if(t0 > 0.0f)
		discrim = t0;
	else
		discrim = t1;

	r->x = a->x + d->x * discrim;
	r->y = a->y + d->y * discrim;
	r->z = a->z + d->z * discrim;

	return true;*/

	
	VECTOR vOToC = *sphereC - *a;
	
	float mag = VectorMagnitude(&vOToC);
	float fHypotenuseSq = mag*mag,
		  fProjection   = DotProduct(&vOToC , d);
	
	// If the projection's negative, then 't' is going to be negative ( if there's a 't' )
	if( fProjection < 0 )
	{
		return false;
	}
	
	//
	// fDistanceSq is (square) the shortest distance between the sphere center & the ray
	//
	float fDistanceSq = fHypotenuseSq - fProjection * fProjection;
	
	// If it's greater than (square) the radius, then there's no intersection
	if(fDistanceSq > sphereR * sphereR)
	{
		return false;
	}
		
	// Calculate 't', the parameter
	float t = fProjection - (float)sqrt( sphereR*sphereR - fDistanceSq);
	
	// If it's less than 0, then we have a straight line-sphere intersection not a
	// ray-sphere intersection
	if( t < 0 )
	{
		return false;
	}
	
	r->x = a->x + d->x * t;
	r->y = a->y + d->y * t;
	r->z = a->z + d->z * t;
	
	return true;
}

_API bool LineIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* start, VECTOR* end)
{
	VECTOR d;
	d.x = end->x - start->x;
	d.y = end->y - start->y;
	d.z = end->z - start->z;
	VectorNormalize(&d, &d);
	return RayIntersectSphere(r, sphereC, sphereR, start, &d);
}