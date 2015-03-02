#ifndef _2D_QUAD_H
#define _2D_QUAD_H

#include "..\Common\fvf.h"
#include "..\Common\types.h"
#include "IQuad.h"

class _API Quad2D : public IQuad
{
public:
	Quad2D();
	virtual ~Quad2D();

	RESULT Render();
	
	RESULT Create(IDevice* device, IGraphObjFactory* factory, TLTVERTEX* quad_verts);
private:
	static IDevice* m_device;
};

#endif