#ifndef TOW_GAMEOBJECTMANAGER_H
#define TOW_GAMEOBJECTMANAGER_H

#include "TOW_GameObject.h"
#include "TOW_GameObjectFactory.h"
#include "..\Common\custmoniker.h"

#include <list>


#define TOW_Manager TOW_ObjectManager

template<class t=TOW_GameObject, class f=TOW_GameObjectFactory>
class TOW_ObjectManager
{
public:
	TOW_ObjectManager();
	~TOW_ObjectManager();

	bool RegisterObject(t* go, moniker& id);
	bool RemoveObject(moniker& id, bool free_me=false);
	t* GetObject(moniker& id);

	void SetConsole(console* c);

	t* CreateObjectOfType(moniker& type, moniker& id);
private:
	console* m_console;
	f m_f;
	struct OC
	{
		moniker id;
		t* obj;
		bool operator == (const OC& rhs)
		{
			if((id() == rhs.id()) && (rhs.obj == obj)) return true;
			return false;
		}
	};
	std::list<OC> objects;
	typedef std::list<OC>::iterator iterator;
};

#endif

