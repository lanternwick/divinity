// Effects.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "..\system\system.h"
#include "..\ui\ui.h"
#include <VECTOR.h>
#include <CObject.h>
#include <result.h>

#include "Effects.h"

#include "Iemitter.h"
#include "IParticleSystem.h"
#include "EFF3DParticleSystem.h"
#include "EFF2DParticleSystem.h"

HMODULE mod;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    mod = (HMODULE)hModule;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			OutputLogString("EFFECTS: :====> ENTER: DLLMAIN() : Process Attach\n\n");
			{
				RegisterModule((HMODULE)hModule);
			}
			OutputLogString("EFFECTS: :====> EXIT: DLLMAIN() : Process Attach\n\n");
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

RESULT EFFECTS_API ConstructEmitter(HANDLE* emitter, BOOL is3D)
{
	IEmitter* em = NULL;
	em = new IEmitter;
	if(em == NULL)
		return OUTOFMEMORY;
	em->_3D = is3D;
	*emitter = Register(em);
	return OK;
}

RESULT EFFECTS_API Construct3DParticleSystem(HANDLE emitter, INT numParticles, VECTOR* Pos, UpdateFunction* ufunc, HANDLE texture, HANDLE attach, DWORD param, BOOL deleteOnDead, HANDLE* PSys)
{
	EFF3DParticleSystem* psys3d = NULL;
	psys3d = new EFF3DParticleSystem;
	if(psys3d == NULL)
		return OUTOFMEMORY;
	psys3d->Create(emitter, numParticles, Pos, ufunc, param, texture, deleteOnDead);
	*PSys = Register(psys3d);
	return OK;
}

RESULT EFFECTS_API Construct2DParticleSystem(HANDLE emitter, INT numParticles, VECTOR* Pos, UpdateFunction* ufunc, HANDLE texture, HANDLE attach, DWORD param, BOOL deleteOnDead, HANDLE* PSys)
{
	EFF2DParticleSystem* psys2d = NULL;
	psys2d = new EFF2DParticleSystem;
	if(psys2d == NULL)
		return OUTOFMEMORY;
	psys2d->Create(emitter, numParticles, Pos, ufunc, param, texture, deleteOnDead);
	*PSys = Register(psys2d);
	return OK;
}

RESULT EFFECTS_API ConstructParticleSystem(HANDLE emitter, INT numParticles, VECTOR* Pos, UpdateFunction* ufunc, HANDLE texture, HANDLE attach, DWORD param, BOOL deleteOnDead, HANDLE* PSys)
{
	RESULT r = OK;
	IEmitter* em = NULL;
	GetObjectPointer(mod, emitter, OBJ_EMITTER, (CObject**)&em);
	if(em == NULL)
		return OBJECTINVALID;
	if(em->_3D)
	{
		r = Construct3DParticleSystem(emitter, numParticles, Pos, ufunc, texture, attach, param, deleteOnDead, PSys);
	}
	else
	{
		r = Construct2DParticleSystem(emitter, numParticles, Pos, ufunc, texture, attach, param, deleteOnDead, PSys);
	}
	return OK;
}

BOOL EFFECTS_API CanInitParticle(HANDLE emitter)
{
	IEmitter* em = NULL;
	GetObjectPointer(mod, emitter, OBJ_EMITTER, (CObject**)&em);
	if(em == NULL)
	{
		return FALSE;
	}
	return TRUE;
}
	