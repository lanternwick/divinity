#include "TOW_GameObjectManager.h"

template<class t, class f>
TOW_ObjectManager<t, f>::TOW_Manager()
{

}

template<class t, class f>
TOW_ObjectManager<t, f>::~TOW_Manager()
{

}

template<class t, class f>
bool TOW_ObjectManager<t, f>::RegisterObject(t* go, moniker& id)
{
	OC oc;
	if(GetObject(id))
		return false;
	oc.id = id;
	oc.obj = go;
	objects.push_back(oc);
	return true;
}

template<class t, class f>
bool TOW_ObjectManager<t, f>::RemoveObject(moniker& id, bool free_me)
{
	for(iterator i = objects.begin(); i != objects.end(); i++)
	{
		if((*i).id() == id())
		{
			if(free_me)
			{
				delete (*i).obj;
			}
			objects.remove((*i));
			return true;
		}
	}
	return false;
}

template<class t, class f>
t* TOW_ObjectManager<t, f>::GetObject(moniker& id)
{
	for(iterator i = objects.begin(); i != objects.end(); i++)
	{
		if((*i).id() == id())
		{
			return (*i).obj;
		}
	}
	return NULL;
}

template<class t, class f>
void TOW_ObjectManager<t, f>::SetConsole(console* c)
{
	m_console = c;
}

template<class t, class f>
t* TOW_ObjectManager<t, f>::CreateObjectOfType(moniker& type, moniker& id)
{
	return NULL;
}