#ifndef _IEMITTER_H
#define _IEMITTER_H

#include <CObject.h>

class IEmitter : public CObject
{
public:
	enum EMTYPE
	{
		_2D = 0,
		_3D,
	};

	IEmitter();
	IEmitter(EMTYPE, VECTOR* pos, InitPointProc* p);
	EMTYPE EmitterType;

	virtual VOID InitParticle(PARTICLE* r);
	VOID SetProc(InitPointProc* p);
	VOID SetPos(VECTOR& nPos);
	VECTOR GetPos();

protected:
	InitPointProc* myProc;
	VECTOR myPos;
};

IEmitter::IEmitter()
{
	type = OBJ_EMITTER;
}

IEmitter::IEmitter(EMTYPE t, VECTOR* pos, InitPointProc* p)
{
	type = OBJ_EMITTER;
	EmitterType = t;
	myPos = *pos;
	myProc = p;
}

VOID IEmitter::InitParticle(PARTICLE* r)
{
	if(myProc)
	{
		myProc(r);
		r->myPos+=myPos;
	}
	else
	{
		r->myPos = myPos;
	}
	return;
}

VOID IEmitter::SetProc(InitPointProc* p)
{
	myProc = p;
}

VOID IEmitter::SetPos(VECTOR& nPos)
{
	myPos = nPos;
	return;
}

VECTOR IEmitter::GetPos()
{
	return myPos;
}


#endif