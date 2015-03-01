#ifndef TOW_SELECTINDICATOR_H
#define TOW_SELECTINDICATOR_H

#pragma warning(disable : 4786)

#include "..\Common\IEntity.h"
#include "..\DivGraph\IDevice.h"
#include "..\DivGraph\IGraphObjFactory.h"
#include "..\Common\IObject.h"
#include "TOW_GameObject.h"
#include "IHeightMap.h"
#include "..\Common\types.h"
#include "..\Common\IDestroyable.h"

class TOW_SelectIndicator : public IEntity
{
public:
	TOW_SelectIndicator();
	~TOW_SelectIndicator();

	RESULT Create(IDevice* device, IGraphObjFactory* factory);

	RESULT Render();

	void SetGameObject(TOW_GameObject* go);

	void Release();

	void Color(unsigned long color);
	unsigned long Color();

	static void SetHeightMap(IHeightMap* hgtmap);
	virtual bool Destroy(float t);
	virtual bool DestroyTick();
private:
	IDevice* m_device;
	IGraphObjFactory* m_factory;
	IVertexBuffer* m_vb;

	TOW_GameObject* m_go;
	unsigned long m_color;
	unsigned long m_current_color;
	float dest_t;
	float r;
	VECTOR p;
	static IHeightMap* m_hgtmap;
};

#endif