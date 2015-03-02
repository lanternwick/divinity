#ifndef _ITHINKER_H
#define _ITHINKER_H

#include "RESULT.h"

class IThinker
{
public:
	virtual ~IThinker() {}
	virtual RESULT Think()=0;
};

#endif