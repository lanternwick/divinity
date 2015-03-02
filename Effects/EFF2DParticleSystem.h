#ifndef _EFF2DPARTICLESYSTEM_H
#define _EFF2DPARTICLESYSTEM_H

#include "IParticleSystem.h"

class EFF2DParticleSystem : public IParticleSystem
{
public:
	EFF2DParticleSystem();
	~EFF2DParticleSystem();

	RESULT Create(HANDLE emitter, INT numParts, VECTOR* Pos, UpdateFunction* UpdateProc, DWORD param1, HANDLE texture, BOOL deleteOnDead);

	RESULT Update();
	RESULT Render(DWORD& numPrims);
private:
	VOID SyncParticleData();
};

EFF2DParticleSystem::EFF2DParticleSystem()
{
	alive = TRUE;
	ret = 0;
}

EFF2DParticleSystem::~EFF2DParticleSystem()
{
	delete particles;
}

RESULT EFF2DParticleSystem::Create(HANDLE emitter, INT numParts, VECTOR* Pos, UpdateFunction* UpdateProc, DWORD param1, HANDLE texture, BOOL deleteOnDead)	
{
	CreateVertexBuffer(&myVB, 6*sizeof(TLTVERTEX)*numParts, sizeof(TLTVERTEX), USAGE_DYNAMIC | USAGE_WRITEONLY, TLTVERTFVF);

	numParticles = numParts;
	particles = new PARTICLE[numParticles];
	ZeroMemory(particles, sizeof(PARTICLE)*numParticles);
	updateFunc = UpdateProc;
	myEmitter = emitter;
	myTexture = texture;
	param = param1;
	SyncParticleData();
	DeleteOnDead = deleteOnDead;
	myPos = *Pos;
	Update();
	return OK;
}

RESULT EFF2DParticleSystem::Update()
{
	return IParticleSystem::Update();
}

RESULT EFF2DParticleSystem::Render(DWORD& numPrims)
{
	
	HANDLE mat = CreateMatrix();
	MatrixIdentity(mat);
	SelectObject(myVB);
	SelectObject(myTexture);
	SetRenderState(RS_ALPHABLENDENABLE, TRUE);
	SyncParticleData();
	if(!alive)
	{
		if(DeleteOnDead)
			deleteFlag = TRUE;	
		return OK;
	}
	SetRenderState(RS_ZWRITEENABLE, FALSE);
	
	//for(register int i = 0; i < ret; ++i)
	{
		//MatrixBillboard(&mat, view, particles[i].myPos.x, particles[i].myPos.y, particles[i].myPos.z);
		//MatrixScaling(matTemp, particles[i].mySize, particles[i].mySize, particles[i].mySize);
		//MatrixMultiply(mat, matTemp, mat);
		SetMatrix(T_WORLD, mat);
		UIBeginDraw();
			DrawTriList(0, ret*2);
		UIEndDraw();
	}
	numPrims+=ret*2;
	//SetRenderState(RS_LIGHTING, FALSE);
	SetRenderState(RS_ALPHABLENDENABLE, FALSE);
	//SetRenderState(RS_FOGENABLE, TRUE);
	SetRenderState(RS_ZWRITEENABLE, TRUE);
	DeleteMatrix(mat);
	return OK;
}

VOID EFF2DParticleSystem::SyncParticleData()
{
	TLTVERTEX* verts;
	
	VECTOR r;
	
	SelectObject(myVB);
	alive = FALSE;
	LockVertexBuffer(0, 0, (BYTE**)&verts, LOCK_DISCARD);
	for(INT i = 0;	i < ret; ++i)
	{
		
		verts[i*6  ] = TLTVertex(particles[i].myPos.x, particles[i].myPos.y, 0.1f, 1.0f, particles[i].color, 0.0f, 0.0f);
		
		
		verts[i*6+1] = TLTVertex(particles[i].myPos.x+particles[i].mySize, particles[i].myPos.y, 0.1f, 1.0f,
								particles[i].color, 1.0f, 0.0f);

		
		verts[i*6+2] = TLTVertex(particles[i].myPos.x+particles[i].mySize, particles[i].myPos.y+particles[i].mySize,0.1f, 1.0f,
								particles[i].color, 1.0f, 1.0f);

		
		verts[i*6+3] = TLTVertex(particles[i].myPos.x+particles[i].mySize, particles[i].myPos.y, 0.1f, 1.0f,
								particles[i].color, 1.0f, 0.0f);

		
		verts[i*6+4] = TLTVertex(particles[i].myPos.x+particles[i].mySize, particles[i].myPos.y+particles[i].mySize,0.1f, 1.0f,
								particles[i].color, 1.0f, 1.0f);
		
		
		verts[i*6+5] = TLTVertex(particles[i].myPos.x, particles[i].myPos.y+particles[i].mySize,0.1f, 1.0f, 
								particles[i].color, 0.0f, 1.0f);
		alive = ( alive || particles[i].alive);
	}
	UnlockVertexBuffer();
	return;
}

#endif