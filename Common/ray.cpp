#include "ray.h"

inline float sq(float x)
{
	return x*x;
}

bool ray::intersect_sphere(VECTOR* r, VECTOR* s_c, float s_r, float* r_t)
{
	/*float ca;
	float cb;
	float cc;
	float discrim;
	float t0;
	float t1;
	VECTOR d;
	VectorNormalize(&d, &dir);
	//ca = d.x * d.x + d.y * d.y + d.z * d.z;
	cb = 2 * (d.x * (orig.x - s_c->x) + d.y * (orig.y - s_c->y) + d.z * (orig.z - s_c->z));
	cc = sq((orig.x - s_c->x)) + sq((orig.y - s_c->y)) + sq((orig.z - s_c->z)) - sq(s_r);
	
	discrim = sq(cb) - 4 * cc;
	if(discrim < 0.0f)
		return false;
	
	t0 = (-1.0f * cb + float(sqrt(discrim)) / 2.0f);
	t1 = (-1.0f * cb - float(sqrt(discrim)) / 2.0f);

	if(t0 < t1)
	{
		if(t0 > 0.0f)
		{
			discrim = t0;
		}
		if(t1 > 0.0f)
		{
			discrim = t1;
		}
	}			
	else
	{
		if(t1 < 0.0f) 
			return false;
		discrim = t1;
	}
	if(r)
	{
		r->x = orig.x + d.x * discrim;
		r->y = orig.y + d.y * discrim;
		r->z = orig.z + d.z * discrim;
	}

	return true;*/

	//if( NULL != pfT )
	//{
	//	*pfT = 0;
	//}
	//
	// vOToC is the position vector of sphere center with respect to ray origin
	//
	VECTOR vOToC = *s_c - orig;
	VECTOR d;
	VectorNormalize(&d, &dir);
	float mag = VectorMagnitude(&vOToC);
	float fHypotenuseSq = mag*mag,
		  fProjection   = DotProduct(&vOToC, &dir);
	
	// If the projection's negative, then 't' is going to be negative ( if there's a 't' )
	if( fProjection < 0 )
	{
		return false;
	}
	
	//
	// fDistanceSq is (square) the shortest distance between the sphere center & the ray
	//
	float fDistanceSq = fHypotenuseSq - (fProjection * fProjection);
	
	// If it's greater than (square) the radius, then there's no intersection
	if(fDistanceSq > s_r * s_r)
	{
		return false;
	}
		
	// Calculate 't', the parameter
	float t = fProjection - float(sqrt(s_r * s_r - fDistanceSq ));
	
	// If it's less than 0, then we have a straight line-sphere intersection not a
	// ray-sphere intersection
	if( t < 0 )
	{
		return false;
	}
	
	if(r)
	{
		r->x = orig.x + d.x * t;
		r->y = orig.y + d.y * t;
		r->z = orig.z + d.z * t;
	}
	if(r_t)
	{
		*r_t = t;
	}

	return true;
}

bool ray::intersect_circle(VECTOR* r, VECTOR* s_c, float s_r, float* r_t)
{
	/*float ca;
	float cb;
	float cc;
	float discrim;
	float t0;
	float t1;
	VECTOR d;
	VectorNormalize(&d, &dir);
	//ca = d.x * d.x + d.y * d.y + d.z * d.z;
	cb = 2 * (d.x * (orig.x - s_c->x) + d.y * (orig.y - s_c->y) + d.z * (orig.z - s_c->z));
	cc = sq((orig.x - s_c->x)) + sq((orig.y - s_c->y)) + sq((orig.z - s_c->z)) - sq(s_r);
	
	discrim = sq(cb) - 4 * cc;
	if(discrim < 0.0f)
		return false;
	
	t0 = (-1.0f * cb + float(sqrt(discrim)) / 2.0f);
	t1 = (-1.0f * cb - float(sqrt(discrim)) / 2.0f);

	if(t0 < t1)
	{
		if(t0 > 0.0f)
		{
			discrim = t0;
		}
		if(t1 > 0.0f)
		{
			discrim = t1;
		}
	}			
	else
	{
		if(t1 < 0.0f) 
			return false;
		discrim = t1;
	}
	if(r)
	{
		r->x = orig.x + d.x * discrim;
		r->y = orig.y + d.y * discrim;
		r->z = orig.z + d.z * discrim;
	}

	return true;*/

	//if( NULL != pfT )
	//{
	//	*pfT = 0;
	//}
	//
	// vOToC is the position vector of sphere center with respect to ray origin
	//
	VECTOR center = *s_c;
	VECTOR origin = orig;
	center.y = 0;
	origin.y = 0;
	VECTOR vOToC = center - origin;
	
	VECTOR d = dir;
	d.y = 0;
	VectorNormalize(&d, &d);
	float mag = VectorMagnitude(&vOToC);
	float fHypotenuseSq = mag*mag,
		  fProjection   = DotProduct(&vOToC, &d);
	
	// If the projection's negative, then 't' is going to be negative ( if there's a 't' )
	if( fProjection < 0 )
	{
		return false;
	}
	
	//
	// fDistanceSq is (square) the shortest distance between the sphere center & the ray
	//
	float fDistanceSq = fHypotenuseSq - (fProjection * fProjection);
	
	// If it's greater than (square) the radius, then there's no intersection
	if(fDistanceSq > s_r * s_r)
	{
		return false;
	}
		
	// Calculate 't', the parameter
	float t = fProjection - float(sqrt(s_r * s_r - fDistanceSq ));
	
	// If it's less than 0, then we have a straight line-sphere intersection not a
	// ray-sphere intersection
	if( t < 0 )
	{
		return false;
	}
	
	if(r)
	{
		r->x = orig.x + d.x * t;
		r->y = orig.y + d.y * t;
		r->z = orig.z + d.z * t;
	}
	if(r_t)
	{
		*r_t = t;
	}

	return true;
}

bool ray::intersect_plane(VECTOR* r, PLANE* p)
{
	float cosAlpha = p->a * dir.x + p->b * dir.y + p->c * dir.z;
	float dot2;
	float t;
	if(fabs(cosAlpha) < 0.01f)
		return false;
	dot2 = p->a * orig.x + p->b * orig.y + p->c * orig.z;
	t = -1 * dot2 / cosAlpha;
	if(t < 0) return false;
	if(r)
	{
		r->x = orig.x + dir.x * t;
		r->y = orig.y + dir.y * t;
		r->z = orig.z + dir.z * t;
	}
	return true;
}