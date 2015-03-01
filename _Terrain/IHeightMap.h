#ifndef _IHEIGHTMAP_H
#define _IHEIGHTMAP_H

#include "..\Common\vector.h"

class IHeightMap
{
public:
	IHeightMap(){};
	virtual ~IHeightMap(){};

	virtual float GetHeight(float x, float z)=0;
	virtual float GetRawHeight(int x, int z)=0;
	virtual bool RayCast(VECTOR* v, VECTOR* src, VECTOR* dir, float detail)=0;
	virtual bool Occluded(VECTOR* v, VECTOR* src, VECTOR* dir)=0;
};

#endif