#ifndef _IPARTICLESYSTEM_H
#define _IPARTICLESYSTEM_H


#include <CObject.h>
#include <fvf.h>
#include <types.h>
#include <utils.h>
#include <IEmitter.h>





class IParticleSystem : public CObject
{
public:
	// Constructor/Destructor
	IParticleSystem();
	virtual ~IParticleSystem();

	RESULT Update();
	virtual RESULT Render(DWORD& numPrims);

	//Function Create() Creates particle system with number of particles
	virtual RESULT Create(HANDLE emitter, INT numParts, VECTOR* Pos, UpdateFunction* UpdateProc, DWORD param1, HANDLE texture, BOOL deleteOnDead);
	
	//Set Texture Function
	VOID SetTexture(HANDLE newText);


	//Attachment Function.  Use to attach a particle system to another Object
	VOID Attach(HANDLE newOwner)
	{
		myOwner = newOwner;
		return;
	}
	VOID SetDeleteOnDead(BOOL s)
	{
		DeleteOnDead = s;
	}
protected:
	HANDLE myVB;
	HANDLE myTexture;
	HANDLE myOwner;

	VECTOR myPos;
	INT numParticles;
	UpdateFunction* updateFunc;
	PARTICLE* particles;
	virtual VOID SyncParticleData();
	HANDLE myEmitter;
	INT ret;
	DWORD param;
	BOOL alive;
	BOOL DeleteOnDead;
};

IParticleSystem::IParticleSystem()
{
	myVB = NULL;
	myTexture = NULL;
	updateFunc = NULL;
	particles = NULL;
	myOwner = NULL;
	type = OBJ_PARTICLESYSTEM;
}

IParticleSystem::~IParticleSystem()
{
	DeleteObj(myVB);
}

RESULT IParticleSystem::Update()
{	
	CObject::Update();
	if(updateFunc != NULL)
	{
		updateFunc(particles, numParticles, &myPos, &ret, param, myEmitter);
	}
	return OK;
}

RESULT IParticleSystem::Render(DWORD& numPrims)
{
	
	return OK;
}

RESULT IParticleSystem::Create(HANDLE emitter, INT numParts, VECTOR* Pos, UpdateFunction* UpdateProc, DWORD param1, HANDLE texture, BOOL deleteOnDead)
{
	
	return OK;
}


VOID IParticleSystem::SetTexture(HANDLE newText)
{
	myTexture = newText;
	return;
}

VOID IParticleSystem::SyncParticleData()
{
	
	return;
}


#endif
