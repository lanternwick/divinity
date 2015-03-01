#ifndef IQUAD_H
#define IQUAD_H

#include "DivGraph.h"
#include "..\Common\IEntity.h"
#include "..\Common\define_exports.h"

class _API IQuad : public IEntity
{
public:
	IQuad(){};
	virtual ~IQuad(){};

	virtual RESULT Render()=0;
	
protected:
	IVertexBuffer* m_vb;
};

#endif
