#ifndef _WALL_H
#define _WALL_H

#include "..\Common\worldobj.h"

class Wall : public WorldObj
{
public:
	Wall();
	~Wall();


private:
	VECTOR endPos;
};

