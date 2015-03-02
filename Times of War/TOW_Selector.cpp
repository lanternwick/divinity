#include "TOW_Selector.h"
#include "..\DivCore\Core.h"
#include "..\Common\singleton.h"


TOW_World* TOW_Selector::m_world = NULL;
IDevice* TOW_Selector::m_device = NULL;
IGraphObjFactory* TOW_Selector::m_factory = NULL;
TOW_Player* TOW_Selector::m_player = NULL;

TOW_Selector::TOW_Selector()
{
	
}

TOW_Selector::TOW_Selector(std::string& objects)
{
	string_tokenizer<> s(objects, std::string(" ;"));
	moniker m;
	TOW_GameObject* obj;
	for(int i = 0; i < s.size(); ++i)
	{
		m() = s.token(i);
		obj = m_world->GetObject(m);
		if(obj)
			this->objects.push_back(m);
	}
}

TOW_Selector::TOW_Selector(VECTOR& ul, VECTOR& lr)
{
	
	m_ul = ul;
	m_lr = lr;
	Compute();
	
}

TOW_Selector::~TOW_Selector()
{
	std::vector<moniker>::iterator i;
	TOW_GameObject* go;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		go = m_world->GetObject((*i));
		if(go)
			go->DispatchOrder(std::string("deselected"));
	}	
}

void TOW_Selector::AddObject(moniker& object)
{
	TOW_GameObject* obj;
	obj = m_world->GetObject(object);
	if(obj)
	{
		objects.push_back(object);
		obj->DispatchOrder(std::string("selected"));
	}
}

void TOW_Selector::AddObject(TOW_GameObject* object)
{
	if(object)
	{
		objects.push_back(object->name());
		/*tsit = new TOW_SelectIndicator;
		tsit->Create(m_device, m_factory);
		tsit->Color(m_player->color);
		tsit->SetGameObject(object);
		TOW_ObjectManager::RegisterObject(tsit);
		indicators.push_back(tsit);*/
		object->DispatchOrder(std::string("selected"));
	}
}

void TOW_Selector::RemoveObject(moniker& object)
{
	std::vector<moniker>::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		if((*i)() == object())
		{
			(*i) = objects.back();
			objects.pop_back();
			break;
		}
	}
}

void TOW_Selector::RemoveObject(TOW_GameObject* object)
{
	std::vector<moniker>::iterator i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		if(m_world->GetObject((*i)) == object)
		{
			(*i) = objects.back();
			objects.pop_back();
			break;
		}
	}
}

void TOW_Selector::DispatchOrder(std::string& order)
{
	int i;
	TOW_GameObject* go;
	for(i = 0; i != objects.size(); ++i)
	{
		go = m_world->GetObject(objects[i]);
		if(!go)
		{
			objects[i] = objects.back();
			objects.pop_back();
			--i;
			continue;
		}
		go->DispatchOrder(order);
	}
}

void TOW_Selector::SetWorld(TOW_World* world)
{
	m_world = world;
}

void TOW_Selector::SetFactory(IGraphObjFactory* factory)
{
	m_factory = factory;
}

void TOW_Selector::SetDevice(IDevice* device)
{
	m_device = device;
}

void TOW_Selector::SetPlayer(TOW_Player* player)
{
	m_player = player;
}

void TOW_Selector::Start(VECTOR& s)
{
	m_ul = s;
}

void TOW_Selector::End(VECTOR& e)
{
	m_lr = e;
}

void TOW_Selector::Compute()
{
	rect<float> rf;
	float f;
	rf.left = m_ul.x;
	rf.right = m_lr.x;
	if(rf.left > rf.right)
	{
		f = rf.left;
		rf.left = rf.right;
		rf.right = f;
	}
	
	rf.top = m_ul.z;
	rf.bottom = m_lr.z;
	if(rf.bottom > rf.top)
	{
		f = rf.bottom;
		rf.bottom = rf.top;
		rf.top = f;
	}
	std::vector<moniker>::iterator j;
	TOW_GameObject* go;
	for(j = objects.begin(); j != objects.end(); j++)
	{
		go = m_world->GetObject((*j));
		if(go)
			go->DispatchOrder(std::string("deselected"));
	}
	objects.clear();
	std::vector<TOW_SelectIndicator*>::iterator k;
	for(k = indicators.begin(); k != indicators.end(); k++)
	{
		singleton<TOW_ObjectManager*>::instance()->Destroy((*k));
	}
	indicators.clear();
	std::vector<moniker> objs(m_world->GetObjectWithin(rf));
	std::vector<moniker>::iterator i;
	//TOW_SelectIndicator* tsit;
	for(i = objs.begin(); i != objs.end(); i++)
	{
		if(m_world->GetObject((*i))->property(std::string("team")).as_if_int() != m_player->team)
			continue;
		objects.push_back((*i));
		/*tsit = new TOW_SelectIndicator;
		tsit->Create(m_device, m_factory);
		tsit->Color(m_player->color);
		tsit->SetGameObject((*i));
		indicators.push_back(tsit);
		TOW_ObjectManager::RegisterObject(tsit);*/
		m_world->GetObject((*i))->DispatchOrder(std::string("selected"));
	}
	
}

RESULT TOW_Selector::Render()
{
	/*std::vector<moniker>::iterator i;
	for(i = indicators.begin(); i != indicators.end(); i++)
	{
		//(*i)->Render();
	}*/
	return OK;
}

int TOW_Selector::Count()
{
	return objects.size();
}

moniker& TOW_Selector::Object(unsigned long index)
{
	return objects[index];
}