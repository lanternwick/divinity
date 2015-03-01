
#include "TOW_World.h"

TOW_World::TOW_World()
{
	m_id = "the_world";
}

TOW_World::~TOW_World()
{
	/*template_iter i;
	for(i = templates.begin(); i != templates.end(); i++)
		delete (*i);*/
	
	
}

int TOW_World::CreateObject(moniker& objTemplate, moniker& objName, TOW_GameObject* boundObj)
{
	/*template_iter i;
	for(i = templates.begin(); i != templates.end(); i++)
	{
		if((*i)->name()() == objTemplate())
		{
			break;
		}
	}*/
	TOW_GameObjectTemplate<>* temp = TOW_TemplateManager::GetTemplate(objTemplate);
	//if(i == templates.end()) return 1;
	if(temp == NULL)
		return 1;
	boundObj->copy(*temp);
	m_con->create_object_ex(boundObj->property(std::string("console_type")).as_if_string(), objName(), boundObj);
	//if(boundObj->renders());
		//m_curr_scene->AddObject((IRenderable*)boundObj);
	boundObj->SetWorld(this);
	objects.push_back(objName);
	m_map->RegisterObject(boundObj);
	boundObj->property(std::string("name")) = objName();
	return 0;
}

int TOW_World::DeleteObject(moniker& objName)
{
	return 0;
}

/*int TOW_World::LoadTemplate(std::string& template_name)
{
	std::string t_path("object_templates\\");
	t_path += template_name;
	t_path += ".obj";
	TOW_GameObjectTemplate<>* temp = NULL;
	temp = new TOW_GameObjectTemplate<>;
	if(!temp) return 1;
	std::ifstream f(t_path.c_str());
	temp->Load(f);
	templates.push_back(temp);
	return 0;
}

int TOW_World::LoadTemplateFromFile(std::string& filename)
{
	TOW_GameObjectTemplate<>* temp = NULL;
	temp = new TOW_GameObjectTemplate<>;
	if(!temp) return 1;
	std::ifstream f(filename.c_str());
	temp->Load(f);
	templates.push_back(temp);
	return 0;
}*/

RESULT TOW_World::Think()
{
	//RESULT r;
	/*for(int i = 0; i < objects.size(); ++i)
	{
		r = objects[i]->Think();
		if(r == DESTROY)
		{
			TOW_ObjectManager::RegisterObject(objects[i]);
			TOW_ObjectManager::Destroy(objects[i]);
			objects[i] = objects.back();
			objects.pop_back();
			--i;
		}
	}*/
	m_map->Think();
	return OK;
}

RESULT TOW_World::Render()
{
	/*obj_iter i;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		if((*i)->renders())
		{
			TOW_GameObjectOutput goo((*i));
			goo.SetDevice(m_device);
			goo.Render();
		}
	}
	std::vector<IEntity*>::iterator j;
	for(j = m_entities.begin(); j != m_entities.end(); j++)
	{
		ITexture::MakeNull(m_device, 0);
		(*j)->Render();
	}*/
	m_map->Render();
	return OK;
}

void TOW_World::SetDevice(IDevice* dev)
{
	m_device = dev;
}

void TOW_World::SetConsole(console* con)
{
	m_con = con;
}

moniker TOW_World::HitTest(ray* ry)
{
	
	std::vector<std::pair<int, float> > intersections;
	float r_t;
	TOW_GameObject* go;
	for(int i = 0; i != objects.size(); ++i)
	{
		try{
			go = (TOW_GameObject*)m_map->GetObject(objects[i]);
			if(!go) continue;
			bool b = go->HitTest(ry, m_map->HeightMap()->GetHeight(go->pos.x, go->pos.z), &r_t);
			if(b)
			{
				intersections.push_back(std::pair<int, float>(i, r_t));
			}
		}
		catch(...)
		{
			return moniker();
		}
	}
	if(intersections.size() == 0)
		return moniker();
	std::pair<int, float> min = intersections[0];
	for(i = 1; i < intersections.size(); ++i)
	{
		if(intersections[i].second < min.second)
			min = intersections[i];
	}
	return objects[min.first];
}

moniker TOW_World::HitTest2D(ray* ry)
{
	
	std::vector<std::pair<int, float> > intersections;
	float r_t;
	TOW_GameObject* go;
	for(int i = 0; i != objects.size(); ++i)
	{
		try{
			go = (TOW_GameObject*)m_map->GetObject(objects[i]);
			if(!go) continue;
			bool b = ry->intersect_circle(NULL, &go->pos, go->property(std::string("bound_sphere_radius")).as_if_float(), &r_t);
			if(b)
			{
				intersections.push_back(std::pair<int, float>(i, r_t));
			}
		}
		catch(...)
		{
			return moniker();
		}
	}
	if(intersections.size() == 0)
		return moniker();
	std::pair<int, float> min = intersections[0];
	for(i = 1; i < intersections.size(); ++i)
	{
		if(intersections[i].second < min.second)
			min = intersections[i];
	}
	return objects[min.first];
}

void TOW_World::SetMap(TOW_Map* Map)
{
	m_map = Map;
}

void TOW_World::SetPathingMap(TOW_PathingMap* p_map)
{
	m_p_map = p_map;
}

void TOW_World::UpdateGameObject(std::string& update_string)
{
	std::string del(" ");
	string_tokenizer<> s(update_string, del);
	std::string objName = s.token(0);
	TOW_GameObject* obj = NULL;
	int i;
	for(i = 0; i < objects.size(); ++i)
	{
		if(objects[i]() == objName)
		{
			obj = (TOW_GameObject*)m_map->GetObject(objects[i]);
			break;
		}
	}
	if(obj == NULL)
		return;
	if(s.token(1) == "move")
	{
		MoveGameObject(obj, s.token(2), s.token(3), s.token(4));
		return;
	}
	if(s.token(1) == "damage")
	{
		DamageGameObject(obj, s.token(2), s.token(3));
	}
}

void TOW_World::MoveGameObject(TOW_GameObject* obj, std::string& type, std::string& x, std::string& y)
{
	float xx, yy;
	xx = atof(x.c_str());
	yy = atof(y.c_str());
	if(type == "rel")
	{
		obj->pos.x += xx;
		obj->pos.z += yy;
		return;
	}
	if(type == "abs")
	{
		obj->pos.x = xx;
		obj->pos.z = yy;
		return;
	}
	return;
}

void TOW_World::DamageGameObject(TOW_GameObject* obj, std::string& attacker, std::string& amount)
{
	int dam = atoi(amount.c_str());
	int hp = obj->property(std::string("current_HP")).as_if_int();
	hp -= dam;
	obj->property(std::string("current_HP")).insert(&hp, sizeof(hp));
	std::stringstream ss;
	ss << "damage_taken " << attacker << ' ' << amount;
	obj->Message(ss.str());
}

TOW_Map* TOW_World::GetMap()
{
	return m_map;
}

TOW_PathingMap* TOW_World::GetPathingMap()
{
	return m_p_map;
}

TOW_GameObject* TOW_World::GetObject(moniker& object_name)
{
	obj_iter i = objects.end();
	for(int j = 0; j < objects.size(); ++j)
	{
		if(objects[j]() == object_name())
		{
			i = &objects[j];
		}
	}
	if(i == objects.end())
		return NULL;
	return (TOW_GameObject*)m_map->GetObject(object_name);
}

std::vector<moniker> TOW_World::GetObjectWithin(rect<float>& bounds, bool rall)
{
	obj_iter i;
	std::vector<moniker> lst;
	std::vector<moniker> all;
	int m = 0;
	TOW_GameObject* go;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		go = (TOW_GameObject*)m_map->GetObject((*i));
		if(!go)
			continue;
		if((go->pos.x >= bounds.left) &&
			(go->pos.x <= bounds.right) &&
			(go->pos.z >= bounds.bottom) &&
			(go->pos.z <= bounds.top))
		{
			m = go->property(std::string("multiselect")).as_if_int();
			if(m)
			{
				lst.push_back((*i));
			}
			m = go->property(std::string("selectable")).as_if_int();
			if(m)
			{
				all.push_back((*i));
			}
		}
	}
	if(lst.size() == 0)
	{
		if(all.size() > 0)
		{
			lst.push_back(all[0]);
		}
	}
	if(rall)
		return all;
	return lst;
}

std::vector<moniker> TOW_World::GetObjectWithinRange(float x, float z, float r, bool rall)
{
	obj_iter i;
	std::vector<moniker> lst;
	std::vector<moniker> all;
	int m = 0;
	float d;
	TOW_GameObject* go;
	for(i = objects.begin(); i != objects.end(); i++)
	{
		go = (TOW_GameObject*)m_map->GetObject((*i));
		if(!go)
			continue;
		d = sqrt((((x - go->pos.x)*(x - go->pos.x))+(((z - go->pos.z)*(z - go->pos.z)))));
		if(d <= r)
		{
			m = go->property(std::string("multiselect")).as_if_int();
			if(m)
			{
				lst.push_back((*i));
			}
			m = go->property(std::string("selectable")).as_if_int();
			if(m)
			{
				all.push_back((*i));
			}
		}
	}
	if(lst.size() == 0)
	{
		if(all.size() > 0)
		{
			lst.push_back(all[0]);
		}
	}
	if(rall)
		return all;
	return lst;
}

void TOW_World::AddObject(IEntity* obj)
{
	m_entities.push_back(obj);
}

void TOW_World::DestroyObject(IEntity* obj)
{
	std::vector<IEntity*>::iterator i = std::find(m_entities.begin(), m_entities.end(), obj);
	if(i == m_entities.end())
		return;
	delete (*i);
	(*i) = m_entities.back();
	m_entities.pop_back();
}