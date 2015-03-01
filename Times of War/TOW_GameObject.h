#ifndef TOW_GAMEOBJECT_H
#define TOW_GAMEOBJECT_H

#pragma warning(disable:4786)

#include "..\Common\IObject.h"
#include "..\Common\IEntity.h"
#include "Console\IConsoleBind.h"
#include "..\Common\custmoniker.h"
#include "TOW_GameObjectTemplate.h"
#include "property_map.h"
#include "..\Common\VECTOR.h"
#include "game_state.h"
#include "..\Common\ray.h"
#include "TOW_SelectIndicator.h"
#include "TOW_Player.h"
#include "..\DivGraph\IMesh.h"
#include "IHeightMap.h"

class TOW_World;

class TOW_GameObject : public IEntity, public IConsoleBind
{
public:
	TOW_GameObject(const char* id);
	virtual ~TOW_GameObject();

	virtual RESULT Think()=0;
	virtual RESULT Render();
	virtual void Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, std::vector<variant>& params)=0;

	void SetConsoleObjType(moniker& consoleObjType);
	moniker& GetConsoleObjType();
	
	void copy(TOW_GameObjectTemplate<>& rhs);
	moniker name();
	bool renders();
	variant& property(std::string& property);
	VECTOR pos;
	void SetWorld(TOW_World* world);
	bool HitTest(ray* ry, float height, float* r_t=NULL);
	void ForceState(game_state* new_state);
	void ClearState();
	virtual void DispatchOrder(std::string order){};
	virtual void Message(std::string& message);
	virtual void Select(){};
	virtual void Deselect(){};
	void SetOwner(TOW_Player* owner);
	virtual bool Destroy(float t){return false;}
	virtual bool DestroyTick(){return false;}
	static void SetDevice(IDevice* device);
	static void SetFactory(IGraphObjFactory* factory);
	static void SetHeightMap(IHeightMap* heightMap);
protected:
	TOW_GameObject();
	moniker m_console_obj_type;
	property_map<> properties;
	TOW_World* m_world;
	game_state* m_state;
	bool selected;
	
	TOW_Player* m_owner;
	static IDevice* m_device;
	static IGraphObjFactory* m_factory;
	static IHeightMap* m_hmp;

};

#endif

