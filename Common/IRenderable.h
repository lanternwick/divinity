#ifndef _IRENDERABLE_H
#define _IRENDERABLE_H

#include "RESULT.h"
#include "define_exports.h"


class _API IRenderable
{
public:
	virtual ~IRenderable(){}
	virtual RESULT Render()=0;
};

#endif