#ifndef IQUAD_H
#define IQUAD_H

#include "IDevice.h"
#include "IRenderable.h"


class IQuad : public IRenderable
{
public:
	IQuad(){};
	virtual ~IQuad(){};

	virtual RESULT Render()=0;
	
protected:
	IVertexBuffer* m_vb;
};

#endif
