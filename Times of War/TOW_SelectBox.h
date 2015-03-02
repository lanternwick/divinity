#ifndef TOW_SELECTBOX_H
#define TOW_SELECTBOX_H

#pragma warning(disable:4786)

#include "..\Common\IEntity.h"
#include "..\DivGraph\IDevice.h"
#include "..\DivGraph\IGraphObjFactory.h"
#include "..\Common\types.h"
#include "TOW_World.h"
#include "..\DivUI\UserInterface.h"

class TOW_SelectBox : public IEntity
{
public:
	TOW_SelectBox();
	~TOW_SelectBox();

	void SetStartPoint(VECTOR& v);
	RESULT Render();
	void Release();
	void SetWidth(float width);
	void SetColor(unsigned long color);

	bool Destroy(float t);
	bool DestroyTick();

	static void SetDevice(IDevice* device);
	static void SetFactory(IGraphObjFactory* factory);
	static void SetWorld(TOW_World* world);
	static void SetCamera(CCamera* cam);
private:
	IVertexBuffer* m_vb;
	static IDevice* m_device;
	static IGraphObjFactory* m_factory;
	VECTOR m_start;
	VECTOR m_end;
	float m_width;
	unsigned long m_color;
	unsigned long m_current_color;
	bool destroying;
	float dest_t;

	static CCamera*  m_camera;
	static TOW_World* m_world;
};

#endif

