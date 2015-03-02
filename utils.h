#ifndef UTILS
#define UTILS

#include <VECTOR.h>
#include <math.h>
#define square(x) (x*x)




/*inline bool VertexInTriangle(VECTOR& vtxPoint,
                             VECTOR& vtx0,
                             VECTOR& vtx1,
                             VECTOR& vtx2)
{
    double dAngle;

    VECTOR vec0;
    VECTOR vec1;
    VECTOR vec2;
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
}*/

inline DWORD FtoDW(float f) { return *((DWORD*)&f); }

inline float rnd()
{
	return float((rand() % 1000)) / 1000;
}


inline float getd2(INT a, INT b, INT c, INT d, INT e)
{
	float h = (float)max(a, b);
	//cout << 'h' << h << endl;
	h = (float)max(h, c);
	//cout << h << endl;
	h = (float)max(h, d);
	//cout << h << endl;
	h = (float)max(h, e);
	return h;
}

//----------------
// Function: DWORD ColorInt(DWORD C1, DWORD C2, float s)
// Desc: Returns a color linearly interpolated between C1 and C2 

DWORD ColorInt(DWORD C1, DWORD C2, float s)
{
	BYTE R1, G1, B1, A1, R2, G2, B2, A2;
	A1 = HIBYTE(HIWORD(C1));
	R1 = LOBYTE(HIWORD(C1));
	G1 = HIBYTE(LOWORD(C1));
	B1 = BYTE(C1);
	A2 = HIBYTE(HIWORD(C2));
	R2 = LOBYTE(HIWORD(C2));
	G2 = HIBYTE(LOWORD(C2));
	B2 = BYTE(C2);

	A1 += BYTE(s * (A2 - A1));
	R1 += BYTE(s * (R2 - R1));
	G1 += BYTE(s * (G2 - G1));
	B1 += BYTE(s * (B2 - B1));
	
	return MAKELONG(MAKEWORD(A1, R1), MAKEWORD(G1, B1));

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
// Function: FLOAT Get3DXZDist(VECTOR P1, VECTOR P2)
// Desc:  Gets a 2D Dist on the XZ Plane from two Vectors

FLOAT Get3DXZDist(VECTOR P1, VECTOR P2)
{
	return float(fabs(float(sqrt(((P1.x - P2.x) * (P1.x - P2.x)) + ((P1.z - P2.z) * (P1.z - P2.z))))));
}

//----------------
// Function: FLOAT Get3DDist(VECTOR P1, VECTOR P2)
// Desc: Returns a distance in 3D space

FLOAT Get3DDist(VECTOR P1, VECTOR P2)
{
	return float(fabs(float(sqrt(((P1.x - P2.x) * (P1.x - P2.x)) + ((P1.y - P2.y) * (P1.y - P2.y)) + ((P1.z - P2.z) * (P1.z - P2.z))))));
}

//----------------
// Function: FLOAT Get3DDistNOSQRT(VECTOR P1, VECTOR P2)
// Desc: Returns a distance in 3D space, without doing a Square Root

FLOAT Get3DDistNOSQRT(VECTOR P1, VECTOR P2)
{
	return float(fabs(float(((P1.x - P2.x) * (P1.x - P2.x)) + ((P1.y - P2.y) * (P1.y - P2.y)) + ((P1.z - P2.z) * (P1.z - P2.z)))));
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
		

#endif