#ifndef _TOW_HEIGHTMAP_H
#define _TOW_HEIGHTMAP_H

#include "IHeightMap.h"
#include "Plane.h"
#include "Intersections.h"
#include <math.h>
#include <stdio.h>
#include <memory.h>

class TOW_HeightMap : public IHeightMap
{
public:
	TOW_HeightMap();
	virtual ~TOW_HeightMap();

	void CreateMap(unsigned long w, unsigned char* buffer);

	virtual float GetHeight(float x, float z);
	virtual float GetRawHeight(int x, int z);
	virtual bool RayCast(VECTOR* v, VECTOR* src, VECTOR* dir, float detail);
	virtual bool Occluded(VECTOR* v, VECTOR* src, VECTOR* dir);
private:
	float m_noise(int x, int y);
	float m_smoothNoise(float x, float y);
	float m_cosInterpolate(float a, float b, float s);
	float m_intNoise(float x, float y);


	unsigned char* map;
	long width;
};

#endif