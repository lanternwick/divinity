#ifndef _MATRIX_H
#define _MATRIX_H

#include <math.h>
#include <VECTOR.h>


struct MATRIX
{
	float _11;
	float _12;
	float _13;
	float _14;

	float _21;
	float _22;
	float _23;
	float _24;

	float _31;
	float _32;
	float _33;
	float _34;

	float _41;
	float _42;
	float _43;
	float _44;
};

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
	result->_22 = cos(t);
	result->_33 = result->_22;
	result->_23 = sin(t);
	result->_32 = -result->_23;
	return;
}

VOID MatrixMatrixRotationY(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_11 = cos(t);
	result->_33 = result->_11;
	result->_31 = sin(t);
	result->_13 = -result->_31;
	return;
}

VOID MatrixMatrixRotationZ(MATRIX* result, float t)
{
	MatrixMatrixIdentity(&result);
	result->_11 = cos(t);
	result->_22 = result->_11;
	result->_12 = sin(t);
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
#endif



			