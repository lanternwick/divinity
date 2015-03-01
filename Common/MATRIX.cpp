
#include "..\Common\matrix.h"
#include "..\Common\vector.h"
#include <windows.h>

VOID MatrixZero(MATRIX** m)
{
	ZeroMemory(*m, sizeof(MATRIX));
	return;
}

VOID MatrixMatrixIdentity(MATRIX** m)
{
	ZeroMemory(*m, sizeof(MATRIX));
	(*m)->_11 = 1.0f;
	(*m)->_22 = 1.0f;
	(*m)->_33 = 1.0f;
	(*m)->_44 = 1.0f;
	return;
}

VOID MatrixMatrixMultiply(MATRIX* result, MATRIX* a, MATRIX* b)
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

VOID MatrixMatrixTranslation(MATRIX* result, float x, float y, float z)
{
	MatrixMatrixIdentity(&result);
	result->_41 = x;
	result->_42 = y;
	result->_43 = z;
	return;
}

VOID MatrixMatrixScaling(MATRIX* result, float x, float y, float z)
{
	MatrixZero(&result);
	result->_11 = x;
	result->_22 = y;
	result->_33 = z;
	result->_44 = 1.0f;
	return;
}

VOID MatrixMatrixRotationX(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_22 = (float)cos(t);
	result->_33 = result->_22;
	result->_23 = (float)sin(t);
	result->_32 = -result->_23;
	return;
}

VOID MatrixMatrixRotationY(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_11 = (float)cos(t);
	result->_33 = result->_11;
	result->_31 = (float)sin(t);
	result->_13 = -result->_31;
	return;
}

VOID MatrixMatrixRotationZ(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_11 = (float)cos(t);
	result->_22 = result->_11;
	result->_12 = (float)sin(t);
	result->_21 = -result->_12;
	return;
}

VOID MatrixMatrixTranspose(MATRIX* result, MATRIX* a)
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

VOID MatrixMatrixBillboardTranspose(MATRIX* result, MATRIX* a, float x, float y, float z)
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

VOID MatrixMatrixVectorApply(VECTOR* r, MATRIX* m, VECTOR* v)
{
	r->x=v->x*m->_11+v->y*m->_21+v->z*m->_31+m->_41;
    r->y=v->x*m->_12+v->y*m->_22+v->z*m->_32+m->_42;
    r->z=v->x*m->_13+v->y*m->_23+v->z*m->_33+m->_43;

	return;
}

VOID MatrixMatrixPerspectiveLH(MATRIX* r, float cx, float cy, float fNear, float fFar)
{
	MatrixZero(&r);

	r->_11 = (2.0f * fNear) / cx;
	r->_22 = (2.0f * fNear) / cy;
	r->_33 = fFar / (fFar - fNear);
	r->_43 = fNear * fFar / (fNear - fFar);
	r->_34 = 1.0f;

	return;
}

VOID MatrixMatrixPerspectiveFovLH(MATRIX* r, float fov, float aspect, float fNear, float fFar)
{
	/*float h = (float)cos(FOV/2.0f) / (float)sin(FOV/2.0f);
	float w = h / aspect;

	

	r->_11 = (2.0f * fNear) / w;
	r->_22 = (2.0f * fNear) / h;
	r->_33 = fFar / (fFar - fNear);
	r->_43 = (fNear * fFar) / (fNear - fFar);
	r->_34 = 1.0f;*/
	
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

VOID MatrixMatrixLookAtLH(MATRIX* r, VECTOR* eye, VECTOR* at, VECTOR* up)
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

	/*
	matrix44 ret;

  vector3 F = target - camPos;
  F.normalize();

  vector3 S = CrossProduct(F, Normalized(camUp));
  S.normalize();

  vector3 U = CrossProduct(S, F);
  U.normalize();

  ret[0][0] = S.x;
  ret[1][0] = S.y;
  ret[2][0] = S.z;
  ret[3][0] = 0.0;

  ret[0][1] = U.x;
  ret[1][1] = U.y;
  ret[2][1] = U.z;
  ret[3][1] = 0.0;

  ret[0][2] = -F.x;
  ret[1][2] = -F.y;
  ret[2][2] = -F.z;
  ret[3][2] = 0.0;

  ret[0][3] = 0.0F;
  ret[1][3] = 0.0F;
  ret[2][3] = 0.0F;
  ret[3][3] = 1.0F;

  ret *= TranslateMatrix44(-camPos.x, -camPos.y, -camPos.z);

  return ret;
  */
	return;
}


