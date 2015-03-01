#ifndef LINE3D_H
#define LINE3D_H

#include "..\Common\IObject.h"
#include "..\Common\IEntity.h"
#include "DivGraph.h"
#include "..\Common\types.h"
#include "render_state.h"

class _API Line3D : public IEntity
{
public:
	Line3D();
	Line3D(VECTOR* s, VECTOR* e);
	~Line3D();

	RESULT Create(IDevice* device, IGraphObjFactory* factory);

	RESULT Render();
	void Start(VECTOR* s);
	void End(VECTOR* e);
	VECTOR Start();
	VECTOR End();
	void Width(float wid);
	float Width();
	void Color(unsigned long color);
	unsigned long Color();

	void Regenerate();

	void Release();
private:
	VECTOR m_s;
	VECTOR m_e;
	IVertexBuffer* m_vb;
	IDevice* m_device;
	IGraphObjFactory* m_factory;
	float m_width;
	unsigned long m_color;
};

#endif