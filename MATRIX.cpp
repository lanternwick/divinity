
#include "matrix.h"
#include "vector.h"
#include <windows.h>

VOID  MatrixZero(MATRIX** m)
{
	ZeroMemory(*m, sizeof(MATRIX));
	return;
}

VOID  MatrixMatrixIdentity(MATRIX** m)
{
	ZeroMemory(*m, sizeof(MATRIX));
	(*m)->_11 = 1.0f;
	(*m)->_22 = 1.0f;
	(*m)->_33 = 1.0f;
	(*m)->_44 = 1.0f;
	return;
}

VOID  MatrixMatrixMultiply(MATRIX* result, MATRIX* a, MATRIX* b)
{
	float val;
	float* r = &(*result)._11;
	float* pa = &(*a)._11;
	float* pb = &(*b)._11;
	for(INT i = 0; i < 4; ++i)
	{
		for(INT j = 0; j < 4; ++j)
		{
			val = 0.0f;
			for(INT x = 0; x < 4; ++x)
			{
				val += pa[i*4+x] * pb[x*4+j];
			}
			r[i*4+j] = val;
		}
		
	}
	return;
}

VOID  MatrixMatrixTranslation(MATRIX* result, float x, float y, float z)
{
	MatrixMatrixIdentity(&result);
	result->_41 = x;
	result->_42 = y;
	result->_43 = z;
	return;
}

VOID  MatrixMatrixScaling(MATRIX* result, float x, float y, float z)
{
	MatrixZero(&result);
	result->_11 = x;
	result->_22 = y;
	result->_33 = z;
	result->_44 = 1.0f;
	return;
}

VOID  MatrixMatrixRotationX(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_22 = (float)cos(t);
	result->_33 = result->_22;
	result->_23 = (float)sin(t);
	result->_32 = -result->_23;
	return;
}

VOID  MatrixMatrixRotationY(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_11 = (float)cos(t);
	result->_33 = result->_11;
	result->_31 = (float)sin(t);
	result->_13 = -result->_31;
	return;
}

VOID  MatrixMatrixRotationZ(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_11 = (float)cos(t);
	result->_22 = result->_11;
	result->_12 = (float)sin(t);
	result->_21 = -result->_12;
	return;
}

VOID  MatrixMatrixTranspose(MATRIX* result, MATRIX* a)
{
	if(result == NULL) return;
	result->_11 = a->_11;
	result->_12 = a->_21;
	result->_13 = a->_31;
	result->_14 = a->_41;

	result->_21 = a->_12;
	result->_22 = a->_22;
	result->_23 = a->_32;
	result->_24 = a->_42;

	result->_31 = a->_13;
	result->_32 = a->_23;
	result->_33 = a->_33;
	result->_34 = a->_43;

	result->_41 = a->_14;
	result->_42 = a->_24;
	result->_43 = a->_34;
	result->_44 = a->_44;
	return;
}

VOID  MatrixMatrixBillboardTranspose(MATRIX* result, MATRIX* a, float x, float y, float z)
{
	if(result == NULL) return;
	result->_11 = a->_11;
	result->_12 = a->_21;
	result->_13 = a->_31;
	result->_14 = 0.0f;

	result->_21 = a->_12;
	result->_22 = a->_22;
	result->_23 = a->_32;
	result->_24 = 0.0f;

	result->_31 = a->_13;
	result->_32 = a->_23;
	result->_33 = a->_33;
	result->_34 = 0.0f;

	result->_41 = x;
	result->_42 = y;
	result->_43 = z;
	result->_44 = a->_44;
	return;
}

VOID  MatrixMatrixVectorApply(VECTOR* r, MATRIX* m, VECTOR* v)
{
	r->x=v->x*m->_11+v->y*m->_21+v->z*m->_31+m->_41;
    r->y=v->x*m->_12+v->y*m->_22+v->z*m->_32+m->_42;
    r->z=v->x*m->_13+v->y*m->_23+v->z*m->_33+m->_43;

	return;
}

VOID  MatrixMatrixPerspectiveLH(MATRIX* r, float cx, float cy, float fNear, float fFar)
{
	MatrixZero(&r);

	r->_11 = (2.0f * fNear) / cx;
	r->_22 = (2.0f * fNear) / cy;
	r->_33 = fFar / (fFar - fNear);
	r->_43 = fNear * fFar / (fNear - fFar);
	r->_34 = 1.0f;

	return;
}

VOID  MatrixMatrixPerspectiveFovLH(MATRIX* r, float fov, float aspect, float fNear, float fFar)
{
	
	
	MatrixZero(&r);

	float    w,h,Q;	
	w = aspect * ( cosf(fov/2.0f)/sinf(fov/2.0f) );
	h = cosf(fov/2.0f)/sinf(fov/2.0f);
	Q = fFar / ( fFar - fNear );
	r->_11 = w;
	r->_22 = h;
	r->_33 = Q;
	r->_34 = 1.0f;
	r->_43 = -Q*fNear;

	return;
}

// NOTE: DO NOT USE!!!!
VOID  MatrixMatrixOrthographicLH(MATRIX* r, float w, float h, float fNear, float fFar)
{
/*
D3DMATRIX M;
float Zf; 
// Far clipping plane
float Zn; // Near clipping plane
float Q = Zf / (Zf - Zn);
M._11 = 1; 
M._12 = 0; 
M._13 =     0; 
M._14 = 0;
M._21 = 1; 
M._22 = 1; 
M._23 =     0; 
M._24 = 0;
M._31 = 1; 
M._32 = 0;
M._33 =     Q; 
M._34 = 0;
M._41 = 1; 
M._42 = 0; 
M._43 = -Q*Zn; 
M._44 = 1;

*/
	//float Q = fFar / (fFar - fNear);
	MatrixZero(&r);
	r->_11 = 50 / w;
	r->_21 = 0;
	r->_22 = 50 / h;
	
	r->_31 = 0;
	r->_33 = fNear / (fFar - fNear);
	
	r->_41 = 0;
	r->_43 = fNear / (fNear + fFar);
	
	r->_44 = 1.0f;

}

VOID  MatrixMatrixLookAtLH(MATRIX* r, VECTOR* eye, VECTOR* at, VECTOR* up)
{
	VECTOR xaxis;
	VECTOR yaxis;
	VECTOR zaxis;

	VECTOR temp;

	temp = *at - *eye;

	VectorNormalize(&zaxis, &temp);

	VectorCross(&temp, up, &zaxis);
	VectorNormalize(&xaxis, &temp);

	VectorCross(&temp, &zaxis, &xaxis);
	VectorNormalize(&yaxis, &temp);

	MatrixMatrixIdentity(&r);
	r->_11 = xaxis.x;
	r->_21 = xaxis.y;
	r->_31 = xaxis.z;

	r->_12 = yaxis.x;
	r->_22 = yaxis.y;
	r->_32 = yaxis.z;

	r->_13 = zaxis.x;
	r->_23 = zaxis.y;
	r->_33 = zaxis.z;

	r->_41 = -1.0f * ((DotProduct(&xaxis, eye)));
	r->_42 = -1.0f * ((DotProduct(&yaxis, eye)));
	r->_43 = -1.0f * ((DotProduct(&zaxis, eye)));

	
	return;
}

// Function uses Cramers Rule, without SIMD Extensions.
// Source: http://developer.intel.com/design/pentiumiii/sml/24504301.pdf
// SIMD Extensions - Inverse of a 4x4 Matrix
void  MatrixMatrixInverse(MATRIX* src, MATRIX* dest)
{
	float* m1 = (float*)src;
	float* dst = (float*)dest;
	
	float tmp[12]; /* temp array for pairs */
	float a[16]; /* array of transpose source matrix */
	float det; /* determinant */
	/* transpose matrix */
	for(int i = 0; i < 4; ++i) 
	{
		a[i] = m1[i*4];
		a[i + 4] = m1[i*4 + 1];
		a[i + 8] = m1[i*4 + 2];
		a[i + 12] = m1[i*4 + 3];
	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = a[10] * a[15];
	tmp[1] = a[11] * a[14];
	tmp[2] = a[9] * a[15];
	tmp[3] = a[11] * a[13];
	tmp[4] = a[9] * a[14];
	tmp[5] = a[10] * a[13];
	tmp[6] = a[8] * a[15];
	tmp[7] = a[11] * a[12];
	tmp[8] = a[8] * a[14];
	tmp[9] = a[10] * a[12];
	tmp[10] = a[8] * a[13];
	tmp[11] = a[9] * a[12];
	/* calculate first 8 elements (cofactors) */
	dst[0] = tmp[0]*a[5] + tmp[3]*a[6] + tmp[4]*a[7];
	dst[0] -= tmp[1]*a[5] + tmp[2]*a[6] + tmp[5]*a[7];
	dst[1] = tmp[1]*a[4] + tmp[6]*a[6] + tmp[9]*a[7];
	dst[1] -= tmp[0]*a[4] + tmp[7]*a[6] + tmp[8]*a[7];
	dst[2] = tmp[2]*a[4] + tmp[7]*a[5] + tmp[10]*a[7];
	dst[2] -= tmp[3]*a[4] + tmp[6]*a[5] + tmp[11]*a[7];
	dst[3] = tmp[5]*a[4] + tmp[8]*a[5] + tmp[11]*a[6];
	dst[3] -= tmp[4]*a[4] + tmp[9]*a[5] + tmp[10]*a[6];
	dst[4] = tmp[1]*a[1] + tmp[2]*a[2] + tmp[5]*a[3];
	dst[4] -= tmp[0]*a[1] + tmp[3]*a[2] + tmp[4]*a[3];
	dst[5] = tmp[0]*a[0] + tmp[7]*a[2] + tmp[8]*a[3];
	dst[5] -= tmp[1]*a[0] + tmp[6]*a[2] + tmp[9]*a[3];
	dst[6] = tmp[3]*a[0] + tmp[6]*a[1] + tmp[11]*a[3];
	dst[6] -= tmp[2]*a[0] + tmp[7]*a[1] + tmp[10]*a[3];
	dst[7] = tmp[4]*a[0] + tmp[9]*a[1] + tmp[10]*a[2];
	dst[7] -= tmp[5]*a[0] + tmp[8]*a[1] + tmp[11]*a[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = a[2]*a[7];
	tmp[1] = a[3]*a[6];
	tmp[2] = a[1]*a[7];
	tmp[3] = a[3]*a[5];
	tmp[4] = a[1]*a[6];
	tmp[5] = a[2]*a[5];

	tmp[6] = a[0]*a[7];
	tmp[7] = a[3]*a[4];
	tmp[8] = a[0]*a[6];
	tmp[9] = a[2]*a[4];
	tmp[10] = a[0]*a[5];
	tmp[11] = a[1]*a[4];
	/* calculate second 8 elements (cofactors) */
	dst[8] = tmp[0]*a[13] + tmp[3]*a[14] + tmp[4]*a[15];
	dst[8] -= tmp[1]*a[13] + tmp[2]*a[14] + tmp[5]*a[15];
	dst[9] = tmp[1]*a[12] + tmp[6]*a[14] + tmp[9]*a[15];
	dst[9] -= tmp[0]*a[12] + tmp[7]*a[14] + tmp[8]*a[15];
	dst[10] = tmp[2]*a[12] + tmp[7]*a[13] + tmp[10]*a[15];
	dst[10]-= tmp[3]*a[12] + tmp[6]*a[13] + tmp[11]*a[15];
	dst[11] = tmp[5]*a[12] + tmp[8]*a[13] + tmp[11]*a[14];
	dst[11]-= tmp[4]*a[12] + tmp[9]*a[13] + tmp[10]*a[14];
	dst[12] = tmp[2]*a[10] + tmp[5]*a[11] + tmp[1]*a[9];
	dst[12]-= tmp[4]*a[11] + tmp[0]*a[9] + tmp[3]*a[10];
	dst[13] = tmp[8]*a[11] + tmp[0]*a[8] + tmp[7]*a[10];
	dst[13]-= tmp[6]*a[10] + tmp[9]*a[11] + tmp[1]*a[8];
	dst[14] = tmp[6]*a[9] + tmp[11]*a[11] + tmp[3]*a[8];
	dst[14]-= tmp[10]*a[11] + tmp[2]*a[8] + tmp[7]*a[9];
	dst[15] = tmp[10]*a[10] + tmp[4]*a[8] + tmp[9]*a[9];
	dst[15]-= tmp[8]*a[9] + tmp[11]*a[10] + tmp[5]*a[8];
	/* calculate determinant */
	det=a[0]*dst[0]+a[1]*dst[1]+a[2]*dst[2]+a[3]*dst[3];
	/* calculate matrix inverse */
	det = 1/det;
	for (int j = 0; j < 16; ++j)
		dst[j] *= det;

}
