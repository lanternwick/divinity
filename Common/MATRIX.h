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


void MatrixZero(MATRIX** m);
void MatrixMatrixIdentity(MATRIX** m);
void MatrixMatrixMultiply(MATRIX* result, MATRIX* a, MATRIX* b);
void MatrixMatrixTranslation(MATRIX* result, float x, float y, float z);
void MatrixMatrixScaling(MATRIX* result, float x, float y, float z);
void MatrixMatrixRotationX(MATRIX* result, float t);
void MatrixMatrixRotationY(MATRIX* result, float t);
void MatrixMatrixRotationZ(MATRIX* result, float t);
void MatrixMatrixTranspose(MATRIX* result, MATRIX* a);
void MatrixMatrixBillboardTranspose(MATRIX* result, MATRIX* a, float x, float y, float z);
void MatrixMatrixVectorApply(VECTOR* r, MATRIX* m, VECTOR* v);
void MatrixMatrixPerspectiveLH(MATRIX* r, float cx, float cy, float fNear, float fFar);
void MatrixMatrixPerspectiveFovLH(MATRIX* r, float fov, float aspect, float fNear, float fFar);
void MatrixMatrixOrthographicLH(MATRIX* r, float w, float h, float fNear, float fFar);
void MatrixMatrixLookAtLH(MATRIX* r, VECTOR* eye, VECTOR* at, VECTOR* up);
void MatrixMatrixInverse(MATRIX* src, MATRIX* dest);

#endif




			