#ifndef TOW_SELECTOR_H
#define TOW_SELECTOR_H

#include "TOW_World.h"
#include "TOW_ObjectManager.h"
#include "Console\string_tokenizer.h"
#include "TOW_SelectIndicator.h"
#include "..\Common\IEntity.h"
#include "TOW_Player.h"

class TOW_Selector : public IEntity
{
public:
	TOW_Selector();
	TOW_Selector(std::string& objects);
	TOW_Selector(VECTOR& ul, VECTOR& lr);
	~TOW_Selector();

	RESULT Render();

	void Start(VECTOR& s);
	void End(VECTOR& e);
	void Compute();
	void AddObject(moniker& object);
	void AddObject(TOW_GameObject* object);
	void RemoveObject(moniker& object);
	void RemoveObject(TOW_GameObject* object);
	int Count();
	moniker& Object(unsigned long index);

	void DispatchOrder(std::string& order);

	static void SetWorld(TOW_World* world);
	static void SetFactory(IGraphObjFactory* factory);
	static void SetDevice(IDevice* device);
	static void SetPlayer(TOW_Player* player);
protected:
	std::vector<moniker> objects;
	std::vector<TOW_SelectIndicator*> indicators;
	static TOW_World* m_world;
	VECTOR m_ul;
	VECTOR m_lr;
	static IDevice* m_device;
	static IGraphObjFactory* m_factory;
	static TOW_Player* m_player;
};

#endif