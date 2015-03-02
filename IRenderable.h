#ifndef _IRENDERABLE_H
#define _IRENDERABLE_H

#include "RESULT.h"

class IRenderable
{
public:
	virtual ~IRenderable(){}
	virtual RESULT Render()=0;
};

#endif