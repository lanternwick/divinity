#include "TOW_ObjectManager.h"
#include "..\DivCore\Core.h"

/*std::vector<IEntity*> TOW_ObjectManager::objects;
std::vector<IUIComponent*> TOW_ObjectManager::windows;
std::vector<IEntity*> TOW_ObjectManager::destroy_list;
IDevice* TOW_ObjectManager::m_device = NULL;*/

void TOW_ObjectManager::Clear(bool free_all)
{
	iterator_t i;
	if(free_all)
	{
		for(i = objects.begin(); i != objects.end(); i++)
		{
			delete (*i);
		}
		for(i = destroy_list.begin(); i != destroy_list.end(); i++)
		{
			delete (*i);
		}
	}
	objects.clear();
	destroy_list.clear();
	//windows.clear();
}

void TOW_ObjectManager::ClearObjects(bool free_all)
{
	iterator_t i;
	if(free_all)
	{
		for(i = objects.begin(); i != objects.end(); i++)
		{
			delete (*i);
		}
		for(i = destroy_list.begin(); i != destroy_list.end(); i++)
		{
			delete (*i);
		}
	}
	objects.clear();
	destroy_list.clear();
}

void TOW_ObjectManager::Destroy(IEntity* obj)
{
	bool b = obj->Destroy(Core::GetTime());
	RemoveObject(obj, false);
	if(!b)
	{
		delete obj;
		return;
	}
	destroy_list.push_back(obj);
	
}

IEntity* TOW_ObjectManager::GetObject(moniker& id)
{
	for(int i = 0; i < objects.size(); ++i)
	{
		if(objects[i]->Identifier()() == id())
			return objects[i];
	}
	return NULL;
}

bool TOW_ObjectManager::RegisterObject(IEntity* obj)
{
	if(std::find(objects.begin(), objects.end(), obj)==objects.end())
	{
		objects.push_back(obj);
		return true;
	}
	return false;
}

/*bool TOW_ObjectManager::RegisterWindow(IUIComponent* comp)
{
	windows.push_back(comp);
	return true;
}*/

bool TOW_ObjectManager::RemoveObject(IEntity* obj, bool free_me)
{
	iterator_t i = std::find(objects.begin(), objects.end(), obj);
	if( i == objects.end())
		return false;
	if(free_me)
		delete (*i);
	(*i) = objects.back();
	objects.pop_back();
	return true;
}

RESULT TOW_ObjectManager::Render()
{
	iterator_t i;
	MATRIX* mat2 = new MATRIX;
	MatrixMatrixIdentity(&mat2);
	
	
	
	for(i = objects.begin(); i != objects.end(); i++)
	{
		m_device->SetTransform(T_WORLD, mat2);
		ITexture::MakeNull(m_device, 0);
		(*i)->Render();
	}
	for(i = destroy_list.begin(); i != destroy_list.end(); i++)
	{
		m_device->SetTransform(T_WORLD, mat2);
		ITexture::MakeNull(m_device, 0);
		(*i)->Render();
	}
	/*for(int j = 0; j < windows.size(); ++j)
	{
		windows[j]->Render();
	}*/
	delete mat2;return OK;
}

RESULT TOW_ObjectManager::Think()
{
	int i;
	RESULT r;
	for(i = 0; i < objects.size(); ++i)
	{
		r = objects[i]->Think();
		if(r == DESTROY)
		{
			Destroy(objects[i]);
			--i;
		}
	}
	/*for(int k = 0; k < windows.size(); ++k)
	{
		windows[k]->Think();
	}*/
	bool b;
	for(int j = 0; j < destroy_list.size(); ++j)
	{
		b = destroy_list[j]->DestroyTick();
		if(!b)
		{
			delete destroy_list[j];
			destroy_list[j] = destroy_list.back();
			destroy_list.pop_back();
			--j;
		}
	}
	return OK;
}

void TOW_ObjectManager::SetDevice(IDevice* device)
{
	m_device = device;
}

int TOW_ObjectManager::size()
{
	return objects.size();
}

IEntity* TOW_ObjectManager::object(int i)
{
	return objects[i];
}