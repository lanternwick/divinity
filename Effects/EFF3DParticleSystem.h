#ifndef _EFF3DPARTICLESYSTEM_H
#define _EFF3DPARTICLESYSTEM_H

#include "IParticleSystem.h"

class EFF3DParticleSystem : public IParticleSystem
{
public:
	EFF3DParticleSystem();
	~EFF3DParticleSystem();

	RESULT Create(HANDLE emitter, INT numParts, VECTOR* Pos, UpdateFunction* UpdateProc, DWORD param1, HANDLE texture, BOOL deleteOnDead);

	RESULT Update();
	RESULT Render(DWORD& numPrims);
private:
	VOID SyncParticleData();
};

EFF3DParticleSystem::EFF3DParticleSystem()
{
	alive = TRUE;
	ret = 0;
}

EFF3DParticleSystem::~EFF3DParticleSystem()
{
	delete particles;
}

RESULT EFF3DParticleSystem::Create(HANDLE emitter, INT numParts, VECTOR* Pos, UpdateFunction* UpdateProc, DWORD param1, HANDLE texture, BOOL deleteOnDead)	
{
	CreateVertexBuffer(&myVB, 6*sizeof(LTVERTEX)*numParts, sizeof(LTVERTEX), USAGE_DYNAMIC | USAGE_WRITEONLY, LTVERTFVF);

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

RESULT EFF3DParticleSystem::Update()
{
	return IParticleSystem::Update();
}

RESULT EFF3DParticleSystem::Render(DWORD& numPrims)
{
	
	HANDLE mat = CreateMatrix();
	MatrixIdentity(mat);
	SelectObject(myVB);
	SelectObject(myTexture);
	SetRenderState(RS_ALPHABLENDENABLE, TRUE);
	SyncParticleData();
	if(alive == FALSE)
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
		DrawTriList(0, ret*2);
	}
	numPrims+=ret*2;
	//SetRenderState(RS_LIGHTING, FALSE);
	SetRenderState(RS_ALPHABLENDENABLE, FALSE);
	//SetRenderState(RS_FOGENABLE, TRUE);
	SetRenderState(RS_ZWRITEENABLE, TRUE);
	DeleteMatrix(mat);
	return OK;
}

VOID EFF3DParticleSystem::SyncParticleData()
{
	LTVERTEX* verts;
	
	VECTOR r;
	HANDLE mat = CreateMatrix();
	HANDLE matTemp = CreateMatrix();
	HANDLE view = CreateMatrix();
	MatrixIdentity(mat);
	MatrixIdentity(matTemp);
	LoadViewMatrix(view);
	
	SelectObject(myVB);
	alive = FALSE;
	LockVertexBuffer(0, 0, (BYTE**)&verts, LOCK_DISCARD);
	for(INT i = 0;	i < ret; ++i)
	{
		MatrixBillboard(&mat, view, particles[i].myPos.x, particles[i].myPos.y, particles[i].myPos.z);
		MatrixScaling(matTemp, particles[i].mySize, particles[i].mySize, particles[i].mySize);
		MatrixMultiply(mat, matTemp, mat);

		MatrixVectorApply(&r, mat, &VECTOR(0.5f, -0.5f, 0.0f));
		verts[i*6  ] = LTVertex(r.x, r.y, r.z, particles[i].color, 1.0f, 1.0f);
		
		MatrixVectorApply(&r, mat, &VECTOR(-0.5f, 0.5f, 0.0f));
		verts[i*6+1] = LTVertex(r.x, r.y, r.z, particles[i].color, 0.0f, 0.0f);

		MatrixVectorApply(&r, mat, &VECTOR(0.5f, 0.5f, 0.0f));
		verts[i*6+2] = LTVertex(r.x, r.y, r.z, particles[i].color, 1.0f, 0.0f);

		MatrixVectorApply(&r, mat, &VECTOR(-0.5f, -0.5f, 0.0f));
		verts[i*6+3] = LTVertex(r.x, r.y, r.z, particles[i].color, 0.0f, 1.0f);

		MatrixVectorApply(&r, mat, &VECTOR(-0.5f, 0.5f, 0.0f));
		verts[i*6+4] = LTVertex(r.x, r.y, r.z, particles[i].color, 0.0f, 0.0f);
		
		MatrixVectorApply(&r, mat, &VECTOR(0.5f, -0.5f, 0.0f));
		verts[i*6+5] = LTVertex(r.x, r.y, r.z, particles[i].color, 1.0f, 1.0f);
		alive = ( alive || particles[i].alive);
	}
	UnlockVertexBuffer();
	DeleteMatrix(mat);
	DeleteMatrix(matTemp);
	DeleteMatrix(view);
	return;
}

#endif