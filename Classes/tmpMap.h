#ifndef _TMPMAP_H
#define _TMPMAP_H

#include <worldobj.h>
#include <result.h>
#include <fvf.h>
#include <types.h>

class TmpMap : public WRLDObject
{
public:
	TmpMap();
	~TmpMap();

	RESULT Create(float cx, float cz, float precis);

	RESULT Render(DWORD& numPrims);
	RESULT Update();
private:
	HANDLE VB;
	DWORD numP;

};

TmpMap::TmpMap()
{

}

TmpMap::~TmpMap()
{

}

RESULT TmpMap::Create(float cx, float cz, float precis)
{
	RESULT r = OK;
	LTVERTEX* verts;
	r = CreateVertexBuffer(&VB, DWORD(((1/precis)*cx) * ((1/precis)*cz) * sizeof(LTVERTEX)*6), sizeof(LTVERTEX), 0, LTVERTFVF);
	if(RFAILED(r))
		return r;
	SelectObject(VB);
	int c = 0 ;
	LockVertexBuffer(0, 0, (BYTE**)&verts, 0);
	{
		for(float i = 0.0f ; i < cx; i+=precis)
		{
			for(float j = 0.0f; j < cz; j+=precis)
			{
				
				verts[c++] = LTVertex(i, 0.0f, j, 0xffffffff, i / cx, j / cz);
				verts[c++] = LTVertex(i+precis, 0.0f, j+precis, 0xffffffff, (i+precis) / cx, (j+precis) / cz);
				verts[c++] = LTVertex(i+precis, 0.0f, j, 0xffffffff, (i+precis) / cx, j / cz);
				
				verts[c++] = LTVertex(i, 0.0f, j, 0xffffffff, i / cx, j / cz);
				verts[c++] = LTVertex(i, 0.0f, j+precis, 0xffffffff, i / cx, (j+precis) / cz);
				verts[c++] = LTVertex(i+precis, 0.0f, j+precis, 0xffffffff, (i+precis) / cx, (j+precis) / cz);
			}
		}
	}
	numP = DWORD(((1/precis)*cx) * ((1/precis)*cz));
	UnlockVertexBuffer();

	return OK;
}

RESULT TmpMap::Render(DWORD& numPrims)
{
	HANDLE mat = CreateMatrix();
	MatrixIdentity(mat);
	SetMatrix(T_WORLD, mat);
	SelectObject(VB);
	DrawTriList(0, numP*2);
	numPrims += numP;
	DeleteMatrix(mat);
	return OK;
}

RESULT TmpMap::Update()
{

	return OK;
}

#endif