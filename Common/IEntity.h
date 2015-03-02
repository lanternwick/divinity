#ifndef _IENTITY_H
#define _IENTITY_H


#include "IThinker.h"
#include "IRenderable.h"
#include "IDestroyable.h"
#include "IObject.h"
#include "define_exports.h"

class _API IEntity : public IObject, public IThinker, public IRenderable, public IDestroyable
{
public:
	virtual RESULT Render(){return OK;};
	virtual RESULT Think(){return OK;};
	
};



#endif