#ifndef TOW_GAMEOBJECTMANAGER_H
#define TOW_GAMEOBJECTMANAGER_H

#pragma warning(disable:4876)

#include "TOW_GameObject.h"

#include "..\Common\IEntity.h"
#include "..\Common\custmoniker.h"
//#include "..\Common\IUIComponent.h"



#include <vector>


class TOW_ObjectManager : public IEntity
{
public:
	
	bool RegisterObject(IEntity* obj);
	//bool RegisterWindow(IUIComponent* comp);
	bool RemoveObject(IEntity* obj, bool free_me=false);
	void Destroy(IEntity* obj);
	IEntity* GetObject(moniker& id);
	typedef std::vector<IEntity*>::iterator iterator_t;
	RESULT Think();
	virtual RESULT Render();
	void Clear(bool free_all=false);
	void ClearObjects(bool free_all=false);
	void SetDevice(IDevice* device);
	int size();
	IEntity* object(int i);
private:
	std::vector<IEntity*> objects;
	//std::vector<IUIComponent*> windows;
	std::vector<IEntity*> destroy_list;
	IDevice* m_device;
};


#endif

