#ifndef _IRENDERABLE_H
#define _IRENDERABLE_H

#include "RESULT.h"

class _API IRenderable
{
public:
	virtual RESULT Render()=0;
};

#endif