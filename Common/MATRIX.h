#ifndef _MATRIX_H
#define _MATRIX_H

/*

Matrix utilities.

These functions are designed for use with Direct3D.  
If you wish to use them with another API, then you must
call MatrixMatrixTranspose() before using them, due to 
the way Direct3D operates.

*/


#include <math.h>
#include "..\Common\VECTOR.h"
#include "..\Common\define_exports.h"

struct _API MATRIX
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


void _API MatrixZero(MATRIX** m);
void _API MatrixMatrixIdentity(MATRIX** m);
void _API MatrixMatrixMultiply(MATRIX* result, MATRIX* a, MATRIX* b);
void _API MatrixMatrixTranslation(MATRIX* result, float x, float y, float z);
void _API MatrixMatrixScaling(MATRIX* result, float x, float y, float z);
void _API MatrixMatrixRotationX(MATRIX* result, float t);
void _API MatrixMatrixRotationY(MATRIX* result, float t);
void _API MatrixMatrixRotationZ(MATRIX* result, float t);
void _API MatrixMatrixTranspose(MATRIX* result, MATRIX* a);
void _API MatrixMatrixBillboardTranspose(MATRIX* result, MATRIX* a, float x, float y, float z);
void _API MatrixMatrixVectorApply(VECTOR* r, MATRIX* m, VECTOR* v);
void _API MatrixMatrixPerspectiveLH(MATRIX* r, float cx, float cy, float fNear, float fFar);
void _API MatrixMatrixPerspectiveFovLH(MATRIX* r, float fov, float aspect, float fNear, float fFar);
void _API MatrixMatrixOrthographicLH(MATRIX* r, float w, float h, float fNear, float fFar);
void _API MatrixMatrixLookAtLH(MATRIX* r, VECTOR* eye, VECTOR* at, VECTOR* up);
void _API MatrixMatrixInverse(MATRIX* src, MATRIX* dest);

#endif




			