#pragma warning(disable:4786)

#include "lua_glue.h"
#include "..\DivCore\Core.h"
#include "TOW_TemplateManager.h"
#include "TOW_ObjectManager.h"
#include "..\DivGraph\IDevice.h"
#include "..\DivGraph\IGraphObjFactory.h"
#include "..\Common\singleton.h"
#include "TOW_Map.h"
#include "TOW_World.h"
#include "TOW_GameUnit.h"
#include "..\DivGraph\StaticMesh.h"
#include "..\DivUI\UICursor.h"
#include "..\DivUI\UserInterface.h"
#include "..\DivGraph\VAnimatedMesh.h"

lua_State* lvm_g;


int lua_tow_init()
{
	lvm_g = lua_open(0);
	if(!lvm_g)
		return 1;
	lua_register(lvm_g, "load_template", lua_load_template);
	lua_register(lvm_g, "load_texture", lua_load_texture);
	lua_register(lvm_g, "remove_media", lua_remove_media);
	lua_register(lvm_g, "clear_templates", lua_clear_templates);
	lua_register(lvm_g, "create_unit", lua_create_unit);
	lua_register(lvm_g, "attach_texture_to_tex_map", lua_attach_texture_to_tex_map);
	lua_register(lvm_g, "load_static_mesh", lua_load_static_mesh);
	lua_register(lvm_g, "set_mesh_dimensions", lua_set_mesh_dimensions);
	lua_register(lvm_g, "create_ui_widget", lua_create_ui_widget);
	lua_register(lvm_g, "load_vanimated_mesh", lua_load_vanimated_mesh);
	lua_register(lvm_g, "set_animation", lua_set_animation);

	return 0;
}

int lua_tow_close()
{
	lua_close(lvm_g);
	return 0;
}

int lua_load_texture(lua_State* vm)
{
	std::string tex_name = lua_tostring(vm, -1);
	
	std::string tex_path = lua_tostring(vm, -2);
	ITexture* tex;
	singleton<IGraphObjFactory*>::instance()->LoadTexture((char*)tex_path.c_str(), tex, (char*)tex_name.c_str());
	lua_pop(vm, 2);
	return 0;
}

int lua_remove_media(lua_State* vm)
{
	std::string media_name = lua_tostring(vm, -1);
	Core::RemoveMediaObject(moniker((char*)media_name.c_str()), true);
	lua_pop(vm, 1);
	return 0;
}

int lua_load_template(lua_State* vm)
{
	std::string template_name = lua_tostring(vm, -1);
	TOW_TemplateManager::LoadTemplate(template_name);	
	lua_pop(vm, 1);
	return 0;
}

int lua_clear_templates(lua_State* vm)
{
	TOW_TemplateManager::Clear(true);
	return 0;
}

int lua_load_mesh(lua_State* vm)
{
	
	return 0;
}

int lua_load_map(lua_State* vm)
{
	TOW_Map* map;
	map = new TOW_Map;
	TOW_World* w = singleton<TOW_World*>::instance();
	if(w->GetMap())
	{
		w->GetMap()->Destroy();
		delete w->GetMap();
	}
	w->SetMap(map);
	std::string map_file = lua_tostring(lvm_g, -1);

	map->Load((char*)map_file.c_str());
	lua_pop(vm, 1);
	return 0;
}

int lua_create_unit(lua_State* vm)
{
	
	TOW_World* w = singleton<TOW_World*>::instance();
	TOW_GameObject* go = new TOW_GameUnit(lua_tostring(vm, -3));;
	go->pos.z = lua_tonumber(vm, -1);
	go->pos.x = lua_tonumber(vm, -2);
	go->pos.y = 0;
	
	w->CreateObject(moniker((char*)lua_tostring(vm, -4)), moniker((char*)lua_tostring(vm, -3)), go); 
	lua_pop(vm, 4);
	return 0;
}

int lua_lua_dobuffer(const char* buffer, size_t size)
{
	return lua_dobuffer(lvm_g, buffer, size, NULL);
}

int lua_attach_texture_to_tex_map(lua_State* vm)
{
	TOW_World* w = singleton<TOW_World*>::instance();
	w->GetMap()->TextureMap()->AttachTexture(moniker((char*)lua_tostring(vm, -2)), lua_tonumber(vm, -1));
	lua_pop(vm, 2);

	return 0;
}
int lua_load_static_mesh(lua_State* vm)
{
	std::string mesh_name = lua_tostring(vm, -1);
	std::string mesh_path = lua_tostring(vm, -2);
	StaticMesh* mesh = new StaticMesh((char*)mesh_name.c_str());
	mesh->SetDevice(singleton<IDevice*>::instance());
	mesh->LoadMesh(singleton<IGraphObjFactory*>::instance(), (char*)mesh_path.c_str());
	Core::RegisterMediaObject(mesh);
	lua_pop(vm, 2);
	return 0;
}

int lua_load_vanimated_mesh(lua_State* vm)
{
	std::string mesh_name = lua_tostring(vm, -1);
	std::string mesh_path = lua_tostring(vm, -2);
	VAnimatedMesh* mesh = new VAnimatedMesh((char*)mesh_name.c_str());
	mesh->SetDevice(singleton<IDevice*>::instance());
	mesh->LoadMesh(singleton<IGraphObjFactory*>::instance(), (char*)mesh_path.c_str());
	Core::RegisterMediaObject(mesh);
	lua_pop(vm, 2);
	return 0;
}

int lua_set_mesh_dimensions(lua_State* vm)
{
	IMesh* mesh = (IMesh*)Core::GetMediaObject(moniker((char*)lua_tostring(vm, -4)));
	float x = lua_tonumber(vm, -3);
	float y = lua_tonumber(vm, -2);
	float z = lua_tonumber(vm, -1);
	if(mesh)
		mesh->SetDimensions(&VECTOR(x, y, z));
	lua_pop(vm, 3);
	return 0;
}

void lua_create_cursor(lua_State* vm)
{
	std::string texture;
	POINT p;
	p.y = lua_tonumber(vm, -1);
	p.x = lua_tonumber(vm, -2);
	texture = lua_tostring(vm, -3);
	UICursor* cur = new UICursor;
	cur->Create(singleton<IDevice*>::instance(), singleton<IGraphObjFactory*>::instance());
	cur->SetCursor(moniker((char*)texture.c_str()));

	cur->SetPos(&p);
	singleton<UserInterface*>::instance()->Windows().AttachWindow(cur);


}

int lua_create_ui_widget(lua_State* vm)
{
	std::string type = lua_tostring(vm, -1);
	if(type == "cursor")
	{
		lua_pop(vm, 1);
		lua_create_cursor(vm);
	}
	return 0;
}

int lua_set_animation(lua_State* vm)
{
	VAnimatedMesh* vanim = (VAnimatedMesh*)Core::GetMediaObject(moniker((char*)lua_tostring(vm, -2)));
	vanim->SetAnimation(moniker((char*)lua_tostring(vm, -1)));
	lua_pop(vm, 2);
	return 0;
}