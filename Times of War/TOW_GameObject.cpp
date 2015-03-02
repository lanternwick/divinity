#include "TOW_GameObject.h"
#include "..\DivCore\Core.h"

IDevice* TOW_GameObject::m_device = NULL;
IGraphObjFactory* TOW_GameObject::m_factory = NULL;
IHeightMap* TOW_GameObject::m_hmp = NULL;

TOW_GameObject::TOW_GameObject()
{

}

TOW_GameObject::TOW_GameObject(const char* id)
{
	m_id() = id;
}

TOW_GameObject::~TOW_GameObject()
{
	ClearState();
}

void TOW_GameObject::SetConsoleObjType(moniker& consoleObjType)
{
	m_console_obj_type = consoleObjType;
}

moniker& TOW_GameObject::GetConsoleObjType()
{
	return m_console_obj_type;
}

void TOW_GameObject::copy(TOW_GameObjectTemplate<>& rhs)
{
	properties = 
		rhs.properties;
}

moniker TOW_GameObject::name()
{
	moniker m;
	try
	{
		m() = properties.property(std::string("name")).as_if_string();
		return m;
	}
	catch(...)
	{
		return moniker("");
	}
}

bool TOW_GameObject::renders()
{
	int ret;
	try
	{
		ret = properties.property(std::string("renders")).as_if_int();
		return ret == 1;
	}
	catch(...)
	{
		return false;
	}
}

variant& TOW_GameObject::property(std::string& property)
{
	return properties.property(property);
}

void TOW_GameObject::SetWorld(TOW_World* world)
{
	m_world = world;
}

bool TOW_GameObject::HitTest(ray* ry, float height, float* r_t)
{
	VECTOR v(pos);
	v.y = height + property(std::string("bound_sphere_center")).as_if_float();

	float rad = property(std::string("bound_sphere_radius")).as_if_float();

	return ry->intersect_sphere(NULL, &v,  rad, r_t);
}

void TOW_GameObject::ForceState(game_state* new_state)
{
	ClearState();
	m_state = new_state;
	m_state->StateEnter();
}

void TOW_GameObject::ClearState()
{
	if(m_state)
	{
		m_state->StateExit();
		delete m_state;
	}
	m_state = NULL;
}

void TOW_GameObject::Message(std::string& message)
{

}

void TOW_GameObject::SetOwner(TOW_Player* owner)
{
	m_owner = owner;
}

RESULT TOW_GameObject::Render()
{
	m_mutex.lock();
	if(!renders())
	{
		m_mutex.unlock();
		return OK;
	}
	moniker mesh_id;
	moniker skin_id;
	try
	{
		mesh_id() = property(std::string("mesh")).as_if_string();
	}
	catch(...)
	{
		m_mutex.unlock();
		return INVALIDDATA;
		//mesh_id = m_go->GetMesh();
	}
	try
	{
		skin_id() = property(std::string("skin")).as_if_string();
	}
	catch(...)
	{
		m_mutex.unlock();
		return INVALIDDATA;
		//skin_id = m_go->GetSkin();
	}
	
	MATRIX m;
	MatrixMatrixTranslation(&m, pos.x, m_hmp ? m_hmp->GetHeight(pos.x, pos.z) + pos.y : pos.y, pos.z);
	ITexture* skin = (ITexture*)Core::GetMediaObject(skin_id);
	IMesh* mesh = (IMesh*)Core::GetMediaObject(mesh_id);
	if(!mesh) 
	{
		m_mutex.unlock();
		return INVALIDDATA;
	}
	if(!skin)
	{
		m_mutex.unlock();
		return INVALIDDATA;
	}
	//MatrixMatrixTranslation(&m, pos.x, pos.y, pos.z);
	//MatrixMatrixMultiply(&m3, &m2, &m);
	skin->Set(0);
	m_device->SetTransform(T_WORLD, &m);
	m_mutex.unlock();
	return mesh->Render();
}

void TOW_GameObject::SetDevice(IDevice* device)
{
	m_device = device;
}

void TOW_GameObject::SetFactory(IGraphObjFactory* factory)
{
	m_factory = factory;
}

void TOW_GameObject::SetHeightMap(IHeightMap* heightMap)
{
	m_hmp = heightMap;
}