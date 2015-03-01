#ifndef TOW_GAMEOBJECTFACTORY_H
#define TOW_GAMEOBJECTFACTORY_H

#pragma warning(disable:4786)

#include "TOW_GameObject.h"
#include "TOW_GameUnit.h"
#include "..\Common\custmoniker.h"

class TOW_GameObjectFactory
{
public:
	TOW_GameObjectFactory();
	~TOW_GameObjectFactory();

	TOW_GameObject* CreateObjectByType(const moniker& type, moniker& id);
};

#endif