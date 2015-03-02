#ifndef _GPMAP_H
#define _GPMAP_H

#define BSIZE 1025

#include <fstream.h>
//#include <GPObject.h>
#include <result.h>
#include <types.h>
//#include system.h>

class GPMap : public CObject
{
public:
	GPMap();
	~GPMap();
	
	RESULT GPMap::Create();
	RESULT Render();
	RESULT Update();
	VOID UpdateMap(int d, int x, int y, float& a);
	INT RenderMap(int d, int x, int y, DWORD& numTris);
	VOID LoadMapFromBMP(char* Filename);
	VOID LoadMapFromDMF(char* Filename);
	VOID Dirty()
	{
		dirty = TRUE;
	}
	VOID Clear()
	{
		for(int i = 0; i < BSIZE; i++)
			for(int j = 0; j < BSIZE; j++)
				Matrix[i][j].hgt = rand() % 4;
	}
	BYTE GetHVal(float x, float z)
	{
		return Matrix[INT(x)][INT(z)].hgt;
	}

protected:
	HANDLE myVB;
	//LPDIRECT3DVERTEXBUFFER8 myVB;
	//LPDIRECT3DTEXTURE8 texture;
	BOOL dirty;
	int lineSize;
	struct hgtMap
	{
		BYTE hgt : 7;
		BYTE on : 1;
	} Matrix[BSIZE][BSIZE];

	//BYTE rottMatrix[BSIZE][BSIZE];
	//BYTE hgt[BSIZE][BSIZE];
	BYTE max;


};

GPMap::GPMap()
{
	myVB = NULL;
	//texture = NULL;
	dirty = TRUE;
}

GPMap::~GPMap()
{
	/*if(myVB != NULL) 
	{
		//myVB->Release();
	}
	if(texture != NULL) texture->Release();*/
}

VOID GPMap::LoadMapFromBMP(char* Filename)
{
	HBITMAP hHgtMap;
	HDC DC;
	//LONG numBytes;
	hHgtMap = (HBITMAP)LoadImage(0, Filename, IMAGE_BITMAP, BSIZE, BSIZE, LR_LOADFROMFILE);
	DC = CreateCompatibleDC(GetDC(GetDesktopWindow()));
	SelectObject(DC, hHgtMap);
	for(INT i = 0; i < BSIZE; ++i)
		for(INT j = 0; j < BSIZE; ++j)
			Matrix[i][j].hgt = GetRValue(GetPixel(DC, i, j)) * 1.5f;

	DeleteDC(DC);
	//GetBitmapBits(hHgtMap, numBytes, (LPVOID)&hgt[0][0]);
	DeleteObject(hHgtMap);
	/*HRESULT hr = D3DXCreateTextureFromFile(Device->Device(), "c:\\divinity\\media\\textures\\terr.tga", &texture);
	switch(hr)
	{
	case D3DXERR_INVALIDDATA:
		MessageBox(0, "invalid", "", MB_OK);
		exit(0);
	case D3DERR_NOTAVAILABLE:
		MessageBox(0, "not availiable", "", MB_OK);
		exit(0);
	case D3DERR_INVALIDCALL:
		MessageBox(0, "bad call", "", MB_OK);
		exit(0);
	case E_OUTOFMEMORY:
		MessageBox(0, "no memory", "", MB_OK);
		exit(0);
	case D3DERR_OUTOFVIDEOMEMORY:
		MessageBox(0, "no v mem", "", MB_OK);
		exit(0);
	case D3D_OK:
		MessageBox(0, "ok", "", MB_OK);
	}*/
	max = 0;
	for(i = 3; i < BSIZE-3; ++i) {
		for(INT j = 3; j < BSIZE-3; ++j){
			Matrix[i][j].hgt = (Matrix[i-1][j-1].hgt + Matrix[i-1][j+1].hgt + Matrix[i+1][j-1].hgt + Matrix[i+1][j+1].hgt + 
						 Matrix[i-2][j-2].hgt + Matrix[i-2][j+2].hgt + Matrix[i+2][j-2].hgt + Matrix[i+2][j+2].hgt + 
						 Matrix[i-3][j-3].hgt + Matrix[i-3][j+3].hgt + Matrix[i+3][j-3].hgt + Matrix[i+3][j+3].hgt + 
						 Matrix[i-1][j].hgt   + Matrix[i+1][j].hgt   + Matrix[i][j-1].hgt   + Matrix[i][j+1].hgt +
						 Matrix[i-2][j].hgt   + Matrix[i+2][j].hgt   + Matrix[i][j-2].hgt   + Matrix[i][j+2].hgt +
						 Matrix[i-3][j].hgt   + Matrix[i+3][j].hgt   + Matrix[i][j-3].hgt   + Matrix[i][j+3].hgt + Matrix[i][j].hgt) / 24;
			if(Matrix[i][j].hgt > max) max = Matrix[i][j].hgt;
			//if(hgt[i][j] == 0) hgt[i][j] = rand() % 2;
		}
	}

	return;
}
	
VOID GPMap::LoadMapFromDMF(char* Filename)
{
	/*ifstream infile(Filename);
	register INT i;
	register INT j;
	for(i = 0; i < BSIZE; ++i)
		for(j = 0; j < BSIZE; ++j)
			infile >> hgt[j][i];
	max = 0;
	for(i = 3; i < BSIZE-3; ++i) {
		for( j = 3; j < BSIZE-3; ++j){
			hgt[i][j] = (hgt[i-1][j-1] + hgt[i-1][j+1] + hgt[i+1][j-1] + hgt[i+1][j+1] + 
						 hgt[i-2][j-2] + hgt[i-2][j+2] + hgt[i+2][j-2] + hgt[i+2][j+2] + 
						 hgt[i-3][j-3] + hgt[i-3][j+3] + hgt[i+3][j-3] + hgt[i+3][j+3] + 
						 hgt[i-1][j]   + hgt[i+1][j]   + hgt[i][j-1]   + hgt[i][j+1] +
						hgt[i-2][j]   + hgt[i+2][j]   + hgt[i][j-2]   + hgt[i][j+2] +
						hgt[i-3][j]   + hgt[i+3][j]   + hgt[i][j-3]   + hgt[i][j+3]) / 24;
			if(hgt[i][j] > max) max = hgt[i][j];
		}
	}*/

	return;
}


RESULT GPMap::Create()
{	
	int cx = BSIZE;
	lineSize = BSIZE / 2;
	RESULT r = CreateVertexBuffer(&myVB, 10*sizeof(MAPVERTEX), sizeof(MAPVERTEX), USAGE_DYNAMIC | USAGE_WRITEONLY, MAPVERTFVF);
	if(FAILED(r))
		return OUTOFMEMORY;
	//HRESULT hr = Device->Device()->CreateVertexBuffer(10*sizeof(MAPVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, MAPVERTFVF, D3DPOOL_SYSTEMMEM, &myVB);
	Update();
	return OK;
}

RESULT GPMap::Update()
{
	CObject::Update();
	if(!dirty) return OK;
	float a = 2.0f;
	//UpdateMap(lineSize, lineSize, lineSize, a);
	UpdateMap(lineSize, lineSize, lineSize, a);
	dirty = FALSE;
	return OK;
}

VOID GPMap::UpdateMap(int d, int x, int y, float& a)
{
	
	static w = 0;
	BYTE b = Matrix[x-d][y-d].hgt;
	float d2 = getd2(abs(b-Matrix[x-d/2][y].hgt),
			 abs(b-Matrix[x+d/2][y].hgt),
			 abs(b-Matrix[x][y].hgt),
			 abs(b-Matrix[x][y-d/2].hgt),
			 abs(b-Matrix[x][y+d/2].hgt));
	if(a > d2) d2 = a;
	VECTOR p;
	GetCameraPos(&p);
	float l = Get3DDist(p, VECTOR(float(x), float(Matrix[x][y].hgt), float(y)));
	//loat f = (l) / ((d) * max(1 * d2, 1));
	float f = l / d;
	float c = 3;
	float d3;
	//d3 = d2;
	if((f < 8) && (d > 1))
	{
		Matrix[x][y].on = 1;
		UpdateMap(d/2, x-d/2, y-d/2, d3);
		UpdateMap(d/2, x-d/2, y+d/2, d3);
		UpdateMap(d/2, x+d/2, y-d/2, d3);
		UpdateMap(d/2, x+d/2, y+d/2, d3);
		
	}
	else
	{
		Matrix[x][y].on = 0;
		a = d2;
	}
	
	return;
}

RESULT GPMap::Render()
{
	HANDLE mat = CreateMatrix();
	MatrixIdentity(mat);
	SetMatrix(T_WORLD, mat);
	DWORD numTris = 0;
	SelectObject(myVB);
	RenderMap(lineSize, lineSize, lineSize,  numTris);
	DeleteMatrix(mat);
	return OK;
}

INT GPMap::RenderMap(int d, int x, int y, DWORD& numTris)
{
	
	//if(Get2DDistABS(x, y, Cam->CameraPos.x, Cam->CameraPos.z) > 100+d) return 1;
	int me = Matrix[x][y].on;
	//if(me == 255) return 1;
	
	if(me == 0)
	{
		//myVB->Unlock();
		return 0;
	}
	
	int _1=0, _2=0, _3=0, _4=0;
	int c = 0;
	if((Matrix[x][y].on == 1) && (d > 1))
	{
		
		_1 = RenderMap(d/2, x-d/2, y-d/2, numTris);
		_4 = RenderMap(d/2, x-d/2, y+d/2, numTris);
		_2 = RenderMap(d/2, x+d/2, y-d/2, numTris);
		_3 = RenderMap(d/2, x+d/2, y+d/2, numTris);
	}
	
	if((_1 + _2 + _3 + _4) == 4) return 1;
	
	MAPVERTEX* t = NULL;
	RESULT r = LockVertexBuffer(0, 0, (BYTE**)&t, LOCK_DISCARD);
	//myVB->Lock(0, 0, (BYTE**)&t, D3DLOCK_DISCARD);
	SetRenderState(RS_CULLMODE, CULL_CW);
	

	t[c++] = MapVertex(float(x), Matrix[x][y].hgt, float(y), 0xff009900, x / BSIZE, y / BSIZE);
	if(_1 == 0)
	{	
		t[c++] = MapVertex(float(x-d), Matrix[x-d][y-d].hgt, float(y-d), 0xff009900, x / BSIZE - d / BSIZE, y / BSIZE - d / BSIZE);
	}
	else
	{
		t[c++] = MapVertex(float(x), Matrix[x][y].hgt, float(y), 0xff009900, x / BSIZE, y / BSIZE);
	}

	if((y-d == 0) || (Matrix[x][y-d*2].on == 1))
	{
		if((_1 == 0) || (_2 == 0))
			t[c++] = MapVertex(float(x), Matrix[x][y-d].hgt, float(y-d), 0xff009900, x / BSIZE , y / BSIZE - d / BSIZE);
	}

	if(_2 == 0)
	{
		t[c++] = MapVertex(float(x+d), Matrix[x+d][y-d].hgt, float(y-d), 0xff009900, x / BSIZE + d / BSIZE, y / BSIZE - d / BSIZE);
	}
	else
	{
		t[c++] = MapVertex(float(x), Matrix[x][y].hgt, float(y),0xff009900, x / BSIZE, y / BSIZE);
	}

	if((x+d == BSIZE -1) || (Matrix[x+d*2][y].on == 1))
	{
		if((_2 == 0) || (_3 == 0))
			t[c++] = MapVertex(float(x+d), Matrix[x+d][y].hgt, float(y), 0xff009900, x / BSIZE + d / BSIZE, y / BSIZE);
	}

	if(_3 == 0)
	{
		t[c++] = MapVertex(float(x+d), Matrix[x+d][y+d].hgt, float(y+d), 0xff009900, x / BSIZE + d / BSIZE, y / BSIZE + d / BSIZE);
	}
	else
	{
		t[c++] = MapVertex(float(x), Matrix[x][y].hgt, float(y), 0xff009900, x / BSIZE, y / BSIZE);
	}

	if((y+d == BSIZE-1) || (Matrix[x][y+d*2].on == 1))
	{
		if((_3 == 0) || (_4 == 0))
			t[c++] = MapVertex(float(x), Matrix[x][y+d].hgt, float(y+d), 0xff009900, x / BSIZE, y / BSIZE + d / BSIZE);
	}

	if(_4 == 0)
	{
		t[c++] = MapVertex(float(x-d), Matrix[x-d][y+d].hgt, float(y+d), 0xff009900, x / BSIZE - d / BSIZE, y / BSIZE + d / BSIZE);
	}
	else
	{
		t[c++] = MapVertex(float(x), Matrix[x][y].hgt, float(y), 0xff009900, x / BSIZE, y / BSIZE);
	}

	if((x-d == 0) || (Matrix[x-d*2][y].on == 1))
	{
		if((_4 == 0) || (_1 == 0))
			t[c++] = MapVertex(float(x-d), Matrix[x-d][y].hgt, float(y), 0xff009900, x / BSIZE - d / BSIZE, y / BSIZE);
	}
	
	if(_1 == 0)
	{
		t[c++] = MapVertex(float(x-d), Matrix[x-d][y-d].hgt, float(y-d), 0xff009900, x / BSIZE - d / BSIZE, y / BSIZE - d / BSIZE);
	}
	else
	{
		t[c++] = MapVertex(float(x-d), Matrix[x-d][y].hgt, float(y), 0xff009900, x / BSIZE - d / BSIZE, y / BSIZE);
		//t[c++] = MapVertex(float(x), hgt[x][y], float(y), 0);
		//t[c++] = MapVertex(x, hgt[x][y], y, 0);
	}
	UnlockVertexBuffer();
	//if(Get2DDistABS(x, y, Cam->CameraPos.x, Cam->CameraPos.z) > 50.0f) return 1;
	if(c > 2)
	{
		DrawTriFan(0, c-2);
		numTris+=c-2;
	}

	SetRenderState(RS_CULLMODE, CULL_CCW);
	return 1;

}

#endif