#ifndef LUA_GLUE_H
#define LUA_GLUE_H

extern "C" {
	#include <lua.h>
}

int lua_tow_init(); // ch
int lua_tow_close(); // ch
int lua_lua_dobuffer(const char* buffer, size_t size); // ch

int lua_load_texture(lua_State* vm); // ch
int lua_remove_media(lua_State* vm); // ch
int lua_load_template(lua_State* vm); // ch
int lua_clear_templates(lua_State* vm); // ch
int lua_load_static_mesh(lua_State* vm); // ch/dbg
int lua_load_mesh(lua_State* vm); // ch
int lua_set_mesh_dimensions(lua_State* vm); // ch


int lua_load_map(lua_State* vm); // ch

int lua_create_unit(lua_State* vm); // ch
int lua_destroy_object(lua_State* vm);

int lua_object_find_path(lua_State* vm);
int lua_generate_pathing(lua_State* vm);
int lua_attach_texture_to_tex_map(lua_State* vm); // ch
int lua_camera_to(lua_State* vm);
int lua_create_ui_widget(lua_State* vm);
int lua_widget_message(lua_State* vm);
int lua_destroy_widget(lua_State* vm);
int lua_play_sound(lua_State* vm);
int lua_play_music(lua_State* vm);
int lua_create_effect(lua_State* vm);
int lua_destroy_effect(lua_State* vm);



#endif

