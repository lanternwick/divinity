#ifndef _IEMITTER_H
#define _IEMITTER_H

#include <CObject.h>

class IEmitter : public CObject
{
public:
	
	IEmitter();
	BOOL _3D;
};

IEmitter::IEmitter()
{
	type = OBJ_EMITTER;
}


#endif