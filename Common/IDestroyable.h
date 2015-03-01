#ifndef IDESTROYABLE_H
#define IDESTROYABLE_H

#include "define_exports.h"

class _API IDestroyable
{
public:
	virtual bool Destroy(float t){return false;};
	virtual bool DestroyTick(){return false;};
};

#endif


