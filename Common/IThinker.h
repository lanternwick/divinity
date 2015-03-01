#ifndef _ITHINKER_H
#define _ITHINKER_H

#include "RESULT.h"
#include "define_exports.h"


class _API IThinker
{
public:
	virtual ~IThinker() {}
	virtual RESULT Think()=0;
};

#endif