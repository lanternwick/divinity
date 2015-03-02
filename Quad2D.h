#ifndef _2D_QUAD_H
#define _2D_QUAD_H

#include "IQuad.h"
#include "fvf.h"
#include "types.h"


class Quad2D : public IQuad
{
public:
	Quad2D();
	virtual ~Quad2D();

	RESULT Render();
	
	RESULT Create(IDevice* device, TLTVERTEX* quad_verts);
private:
	static IDevice* m_device;
};

#endif