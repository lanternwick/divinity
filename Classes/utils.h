#ifndef UTILS
#define UTILS

#include <math.h>
#include <VECTOR.h>
#include <stdlib.h>
#include <fvf.h>
#define square(x) (x*x)
/*
inline bool VertexInTriangle(D3DXVECTOR3& vtxPoint,
                             D3DXVECTOR3& vtx0,
                             D3DXVECTOR3& vtx1,
                             D3DXVECTOR3& vtx2)
{
    double dAngle;

    D3DXVECTOR3 vec0;
    D3DXVECTOR3 vec1;
    D3DXVECTOR3 vec2;
	//if(vtx0.x - INT(vtx0.x) < 0.01)
	D3DXVec3Normalize(&vec0, &(vtxPoint - vtx0));
	D3DXVec3Normalize(&vec1, &(vtxPoint - vtx1));
	D3DXVec3Normalize(&vec2, &(vtxPoint - vtx2));
    dAngle =
        acos(D3DXVec3Dot(&vec0, &vec1)) + 
        acos(D3DXVec3Dot(&vec1, &vec2)) + 
        acos(D3DXVec3Dot(&vec2, &vec0));


    if(fabs(dAngle - 2*D3DX_PI) < 0.1f)
        return true;
    else
        return false;
}
*/
inline DWORD FtoDW(float f) { return *((DWORD*)&f); }

inline float rnd()
{
	return float((rand() % 1000)) / 1000;
}


inline float getd2(INT a, INT b, INT c, INT d, INT e)
{
	float h = (float)max(a, b);
	//cout << 'h' << h << endl;
	h = max(h, c);
	//cout << h << endl;
	h = max(h, d);
	//cout << h << endl;
	h = max(h, e);
	return h;
}

//----------------
// Function: DWORD ColorInt(DWORD C1, DWORD C2, float s)
// Desc: Returns a color linearly interpolated between C1 and C2 

DWORD ColorInt(DWORD C1, DWORD C2, float s)
{
	BYTE R1, G1, B1, A1, R2, G2, B2, A2;
	A1 = BYTE(C1 >> 24);
	A2 = BYTE(C2 >> 24);
	R1 = BYTE(C1 >> 16);
	R2 = BYTE(C2 >> 16);
	G1 = BYTE(C1 >> 8);
	G2 = BYTE(C2 >> 8);
	B1 = BYTE(C1);
	B2 = BYTE(C2);

	A1 += BYTE(s * (A2 - A1));
	R1 += BYTE(s * (R2 - R1));
	G1 += BYTE(s * (G2 - G1));
	B1 += BYTE(s * (B2 - B1));
	
	return ((A1 << 24) + (R1 << 16) + (G1 << 8) + B1);

}

float FloatInt(float f1, float f2, float s)
{
	return f1 + s * (f2 - f1);
}

//----------------
// Function: FLOAT Get2DDist(POINT P1, POINT P2) and FLOAT Get2DDist(float x1, float y1, float x2, float y2)
// Desc: Overloaded Functions to get Distances in 2D space

FLOAT Get2DDist(POINT P1, POINT P2)
{
	return float(sqrt(((P1.x - P2.x) * (P1.x - P2.x)) + ((P1.y - P2.y) * (P1.y - P2.y))));
}

FLOAT Get2DDist(float x1, float y1, float x2, float y2)
{
	return float(sqrt(square((x2 - x1)) + square((y2 - y1))));
}

FLOAT Get2DDistABS(float x1, float y1, float x2, float y2)
{
	return float(fabs(sqrt(square((x2 - x1)) + square((y2 - y1)))));
}

FLOAT Get2DDistNOSQRT(float x1, float y1, float x2, float y2)
{
	return float(square((x2 - x1)) + square((y2 - y1)));
}

//----------------
// Function: FLOAT Get3DXZDist(D3DXVECTOR3 P1, D3DXVECTOR3 P2)
// Desc:  Gets a 2D Dist on the XZ Plane from two Vectors

FLOAT Get3DXZDist(VECTOR P1, VECTOR P2)
{
	return float(fabs(float(sqrt(((P1.x - P2.x) * (P1.x - P2.x)) + ((P1.z - P2.z) * (P1.z - P2.z))))));
}

//----------------
// Function: FLOAT Get3DDist(D3DXVECTOR3 P1, D3DXVECTOR3 P2)
// Desc: Returns a distance in 3D space

FLOAT Get3DDist(VECTOR P1, VECTOR P2)
{
	return float(fabs(float(sqrt(((P1.x - P2.x) * (P1.x - P2.x)) + ((P1.y - P2.y) * (P1.y - P2.y)) + ((P1.z - P2.z) * (P1.z - P2.z))))));
}

/*RESULT HRtoR(HRESULT hr)
{
	switch(hr)
	{
	case D3D_OK:
		return OK;


	default:
		return E_UNKNOWN;
	}
}*/
		

BOOL RayIntersectPlane(VECTOR* r, PLANE* p, VECTOR* a, VECTOR* d)
{
	float cosAlpha = p->a * d->x + p->b * d->y + p->c * d->z;
	float dot2;
	float t;
	if(fabs(cosAlpha) < 0.01f)
		return FALSE;
	dot2 = p->a * a->x + p->b * a->y + p->c * a->z;
	t = -1 * dot2 / cosAlpha;
	if(t < 0) return FALSE;
	r->x = a->x + d->x * t;
	r->y = a->y + d->y * t;
	r->z = a->z + d->z * t;
	return TRUE;
}

BOOL LineIntersectPlane(VECTOR* r, PLANE* p, VECTOR* start, VECTOR* end)
{
	VECTOR d;
	d.x = end->x - start->x;
	d.y = end->y - start->y;
	d.z = end->z - start->z;
	VectorNormalize(&d, &d);
	return RayIntersectPlane(r, p, start, &d);
}

BOOL RayIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* a, VECTOR* d)
{
	float ca;
	float cb;
	float cc;
	float discrim;
	float t0;
	float t1;

	ca = d->x * d->x + d->y * d->y + d->z * d->z;
	cb = 2 * (d->x * (a->x - sphereC->x) + d->y * (a->y - sphereC->y) + d->z * (a->z - sphereC->z));
	cc = square((a->x - sphereC->x)) + square((a->y - sphereC->y)) + square((a->z - sphereC->z)) - square(sphereR);
	
	discrim = square(cb) - 4 * cc;
	if(discrim < 0.0f)
		return FALSE;
	
	t0 = (-1.0f * cb + float(sqrt(discrim)) / 2.0f);
	t1 = (-1.0f * cb - float(sqrt(discrim)) / 2.0f);

	if(t0 > 0.0f)
		discrim = t0;
	else
		discrim = t1;

	r->x = a->x + d->x * discrim;
	r->y = a->y + d->y * discrim;
	r->z = a->z + d->z * discrim;

	return TRUE;
}

BOOL LineIntersectSphere(VECTOR* r, VECTOR* sphereC, float sphereR, VECTOR* start, VECTOR* end)
{
	VECTOR d;
	d.x = end->x - start->x;
	d.y = end->y - start->y;
	d.z = end->z - start->z;
	VectorNormalize(&d, &d);
	return RayIntersectSphere(r, sphereC, sphereR, start, &d);
}



#endif