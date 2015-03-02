#ifndef PT_H
#define PT_H

struct pt
{
	float x;
	float y;
#ifdef PT_3D
	float z;
#endif
};

#endif