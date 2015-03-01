#ifndef TOW_GAMEUNIT_H
#define TOW_GAMEUNIT_H

#pragma warning(disable : 4786)

#include "TOW_GameObject.h"
#include "..\Common\custmoniker.h"
#include "..\Common\Vector.h"
#include "property_map.h"
#include "..\DivCore\log_stream.h"
#include "moving_state.h"
#include "attack_state.h"
#include "TOW_Projectile.h"
#include "TOW_ObjectManager.h"

class TOW_GameUnitOutput;

class TOW_GameUnit : public TOW_GameObject
{
	friend class TOW_GameUnitOutput;
public:
	TOW_GameUnit(const char* id);
	virtual ~TOW_GameUnit();

	virtual RESULT Think();
	//virtual RESULT Render(){return OK;};
	moniker GetMesh();
	void SetMesh(moniker& mesh);
	moniker GetSkin();
	void SetSkin(moniker& skin);
	

	virtual void Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, std::vector<variant>& params);
	virtual void DispatchOrder(std::string order);
	void Message(std::string& message);
	virtual void Select();
	virtual void Deselect();
	virtual bool Destroy(float t);
	virtual bool DestroyTick();
protected:
	TOW_GameUnit();
	moniker m_mesh;
	moniker m_skin;
	TOW_SelectIndicator* m_selind;
	//property_map<> properties;
	float dest_t;
	
};

#endif