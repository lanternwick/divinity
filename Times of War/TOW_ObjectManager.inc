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
	if(m_console)
	{
		m_console->create_object_ex(go->GetConsoleObjType()(), id(), go);
	}
	

	return true;
}

template<class t, class f>
bool TOW_ObjectManager<t, f>::RemoveObject(moniker& id, bool free_me)
{
	for(iterator_t i = objects.begin(); i != objects.end(); i++)
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
	for(iterator_t i = objects.begin(); i != objects.end(); i++)
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
	return m_f->CreateObjectOfType(type, id);
}

template<class t, class f>
TOW_ObjectManager<t, f>::iterator TOW_ObjectManager<t, f>::begin()
{
	return TOW_ObjectManager<t, f>iterator(objects.begin());
}

template<class t, class f>
TOW_ObjectManager<t, f>::iterator TOW_ObjectManager<t, f>::end()
{
	return TOW_ObjectManager<t, f>iterator(objects.end());
}