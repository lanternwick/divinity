#ifndef _WRLDGRASS_H
#define _WRLDGRASS_H

#include <WorldObj.h>

class WRLDGrass : public WRLDObject
{
public:
	WRLDGrass();
	~WRLDGrass();

	RESULT Render();
	RESULT Update();
	RESULT Create();
	VOID Dirty();
	VECTOR* v;
	float* fac;
private:
	HANDLE VB;
	float rot;
	bool dirty;
	VOID Sync();
};

WRLDGrass::WRLDGrass()
{

}

WRLDGrass::~WRLDGrass()
{
	DeleteObj(VB);
}

RESULT WRLDGrass::Render()
{
	HANDLE mat = CreateMatrix();
	HANDLE mat2 = CreateMatrix();
	HANDLE matr = CreateMatrix();
	SetRenderState(RS_ALPHABLENDENABLE, TRUE);
	MatrixTranslation(mat, v->x, 0.0f, v->z);
	MatrixRotationY(mat2, -(*fac)+(3.14159f / 4.0f));
	MatrixMultiply(matr, mat2, mat);
	//MatrixIdentity(matr);
	SetMatrix(T_WORLD, matr);
	SelectObject(VB);
	DrawTriList(0, 3600*2);
	SetRenderState(RS_ALPHABLENDENABLE, FALSE);
	DeleteMatrix(mat);
	DeleteMatrix(mat2);
	DeleteMatrix(matr);
	return OK;
}

RESULT WRLDGrass::Update()
{
	
	{
		Sync();
		dirty = FALSE;
	}
	return OK;
}

RESULT WRLDGrass::Create()
{
	CreateVertexBuffer(&VB, 3600*6 * sizeof(LVERTEX), sizeof(LVERTEX), USAGE_DYNAMIC, LVERTFVF);
	dirty = true;
	Sync();

	return OK;
}

VOID WRLDGrass::Dirty()
{
	dirty = TRUE;
}

VOID WRLDGrass::Sync()
{
	RESULT r;
	LVERTEX* v = NULL;
	static float wind = (rnd() * 2.0f - 1.0f) / 100.0f;
	bool d;
	wind += (rnd() * 2.0f - 1.0f) / 100.0f;
	if(fabs(wind) >= 0.1f)
	{
		wind/=-2.0f;
		d = true;
	}
	if(RFAILED(SelectObject(VB)))
		return;
	r = LockVertexBuffer(0, 0, (BYTE**)&v, LOCK_DISCARD);
	if(RFAILED(r)) return;

	DWORD c = 0;
	float randNum, randNum2;
	while(c < 3600*6)
	{
		randNum = rnd() * 1.3f - 0.3f;
		randNum2 = rnd() * 1.3f - 0.3f;
		if(dirty)
		{
			v[c++] = LVertex(randNum*15.0f, 0.0f, randNum2*15.0f, 0xff004400); //((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
			v[c++] = LVertex(randNum*15.0f+0.03f, 0.0f, randNum2*15.0f+0.02f, 0xff004400); //((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
			v[c++] = LVertex(randNum*15.0f+0.015f+wind, 0.1f, randNum2*15.0f+0.015f+wind, 0xff004400); // ((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
		}
		else
		{
			
			if(d)
			{
				v[c++] = LVertex(randNum*15.0f, 0.0f, randNum2*15.0f, 0xff004400); //((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
				v[c++] = LVertex(randNum*15.0f+0.03f, 0.0f, randNum2*15.0f+0.02f, 0xff004400); //((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
				v[c++] = LVertex(randNum*15.0f+0.015f+wind, 0.1f, randNum2*15.0f+0.015f+wind, 0xff004400); // ((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
			}
			else
			{
				c+=2;
				v[c++] = LVertex(v[c].x+wind, 0.1f, v[c].z+wind, 0xff004400); // ((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
			}
		}
		//v[c++] = LVertex(randNum*15.0f+0.015f+wind, 0.1f, randNum2*15.0f+0.015f+wind, 0xff002200); // ((BYTE((1.0f / float(sqrt(square(randNum) + square(randNum2))))) * 0xff)<<24)+0x009900);
	}
}

#endif