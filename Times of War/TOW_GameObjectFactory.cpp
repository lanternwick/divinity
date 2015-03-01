#include "TOW_GameObjectFactory.h"

TOW_GameObjectFactory::TOW_GameObjectFactory()
{

}

TOW_GameObjectFactory::~TOW_GameObjectFactory()
{

}

TOW_GameObject* TOW_GameObjectFactory::CreateObjectByType(const moniker& type, moniker& id)
{
	if(type() == std::string("unit"))
		return new TOW_GameUnit(id().c_str());
	//if(id() == "building");
	return NULL;
}