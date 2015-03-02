#ifndef TOW_WORLD_H
#define TOW_WORLD_H

#pragma warning (disable : 4786)

#include <vector>

#include "TOW_TemplateManager.h"
#include "TOW_GameObject.h"
#include "TOW_PathingMap.h"
#include "TOW_GameObjectTemplate.h"

#include "..\Common\custmoniker.h"

#include "..\Common\IEntity.h"

#include "..\DivGraph\CScene.h"
#include "..\console\console.h"
#include "TOW_Map.h"
#include "..\Common\string_tokenizer.h"
#include "..\Common\rect_temp.h"
#include "TOW_ObjectManager.h"


class TOW_World : public IEntity
{
public:
	TOW_World();
	~TOW_World();

	CreateObject(moniker& objTemplate, moniker& objName, TOW_GameObject* boundObj);
	DeleteObject(moniker& objName);
	LoadTemplateFromFile(std::string& filename);
	LoadTemplate(std::string& template_name);

	RESULT Render();
	RESULT Think();
	
	void SetDevice(IDevice* device);
	void SetConsole(console* con);

	moniker HitTest(ray* ry);
	moniker HitTest2D(ray* ry);
	TOW_GameObject* GetObject(moniker& object_name);
	std::vector<moniker> GetObjectWithin(rect<float>& bounds, bool rall=false);
	std::vector<moniker> GetObjectWithinRange(float x, float z, float r, bool rall=false);
	void UpdateGameObject(std::string& update_string);
	void SetMap(TOW_Map* Map);
	void SetPathingMap(TOW_PathingMap* p_map);
	TOW_Map* GetMap();
	TOW_PathingMap* GetPathingMap();
	void AddObject(IEntity* obj);
	void DestroyObject(IEntity* obj);
	
private:
	void MoveGameObject(TOW_GameObject* obj, std::string& type, std::string& x, std::string& y);
	void DamageGameObject(TOW_GameObject* obj, std::string& attacker, std::string& amount);
	std::vector<IEntity*> m_entities;
	std::vector<moniker> objects;
	//std::vector<TOW_GameObjectTemplate<>*> templates;
	typedef std::vector<moniker>::iterator obj_iter;
	//typedef std::vector<TOW_GameObjectTemplate<>*>::iterator template_iter;
	IDevice* m_device;
	console* m_con;
	TOW_Map* m_map;
	TOW_PathingMap* m_p_map;
};

#endif