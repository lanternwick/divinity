#include "TOW_TemplateManager.h"


std::vector<TOW_GameObjectTemplate<>* > TOW_TemplateManager::templates;

void TOW_TemplateManager::Clear(bool free_all)
{
	std::vector<TOW_GameObjectTemplate<>* >::iterator i;
	if(free_all)
	{
		for(i = templates.begin(); i != templates.end(); i++)
		{
			delete (*i);
		}
	}
	templates.clear();
}

void TOW_TemplateManager::LoadTemplate(std::string& template_name)
{
	std::string t_path("object_templates\\");
	t_path += template_name;
	t_path += ".obj";
	TOW_GameObjectTemplate<>* temp = NULL;
	temp = new TOW_GameObjectTemplate<>;
	std::ifstream f(t_path.c_str());
	temp->Load(f);
	templates.push_back(temp);
	return;
}

void TOW_TemplateManager::LoadTemplateFromFile(std::string& filename)
{
	TOW_GameObjectTemplate<>* temp = NULL;;
	temp = new TOW_GameObjectTemplate<>;
	std::ifstream f(filename.c_str());
	temp->Load(f);
	templates.push_back(temp);
	return;
}

TOW_GameObjectTemplate<>* TOW_TemplateManager::GetTemplate(moniker& template_name)
{
	std::vector<TOW_GameObjectTemplate<>* >::iterator i;
	for(i = templates.begin(); i != templates.end(); i++)
	{
		if((*i)->name()() == template_name())
		{
			return (*i);
		}
	}
	return NULL;
}

/*moniker TOW_TemplateManager::CreateObject(moniker& objTemplate, moniker& objName)
{
	std::vector<TOW_GameObjectTemplate<> >::iterator i;
	for(i = templates.begin(); i != templates.end(); i++)
	{
		if((*i).name()() == objTemplate())
		{
			break;
		}
	}
	if(i == templates.end()) return moniker("");
	TOW_GameObject* obj;
	std::string type = (*i).property(std::string("native_type")).as_if_string();
	if(type == "TOW_GameUnit")
	{
		obj = new TOW_GameUnit(objName().c_str());
	}
	obj->copy((*i));
	m_console->create_object_ex(obj->property(std::string("console_type")).as_if_string(), objName(), obj);
	//if(boundObj->renders());
		//m_curr_scene->AddObject((IRenderable*)boundObj);
	obj->SetWorld(m_world);
	TOW_ObjectManager::RegisterObject(obj);
	obj->property(std::string("name")) = objName();
	return objName;
}*/