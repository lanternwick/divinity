#ifndef TOW_CONSOLE_FUNCTORS_H
#define TOW_CONSOLE_FUNCTORS_H

#include "TOW_Functor.h"
#include "TOW_World.h"
#include <fstream>
#include "..\DivUI\UICommandMap.h"
#include "..\TOW_Selector.h"
#include "..\TOW_SelectBox.h"
#include "..\TOW_Player.h"
#include "..\tow_networking.h"
#include "..\TOW_CameraController.h"
#include "..\map_load_thread.h"
#include "..\DivUI\UIConsoleWnd.h"

/* functor def

class _functor : public TOW_Functor
{
public:
	  _functor(IDevice* device)
	{
		m_device = device;
	}
	void f_call(console* c, class_data* _this, const class_def* def, custom::vector<variant>& parameters)
	{
		
	}
};

*/


class wireframe_functor : public TOW_Functor
{
public:
	wireframe_functor(IDevice* device)
	{
		m_device = device;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		render_state<RS_FILLMODE, FILL_WIREFRAME> rs(m_device);
	}
};

class solid_functor : public TOW_Functor
{
public:
	solid_functor(IDevice* device)
	{
		m_device = device;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		render_state<RS_FILLMODE, FILL_SOLID> rs(m_device);
	}
};

class quit_functor : public TOW_Functor
{
public:
	quit_functor(bool* running)
	{
		m_running = running;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		*m_running = false;
		if(parameters.size() == 1)
		{
			Core::WriteLog("Quitting!\n", 0);
		}
	}
private:
	bool* m_running;
};

class set_prompt_functor : public TOW_Functor
{
public:
	set_prompt_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		/*UIConsoleWnd* uw;	
		uw = (UIConsoleWnd*)Core::GetMediaObject(moniker("console"));
		uw->SetPrompt(parameters[0].as_if_string());*/
		
	}
	void set_prompt(std::string* s_p)
	{
		m_prompt = s_p;
	}
private:
	std::string* m_prompt;
};

class clear_functor : public TOW_Functor
{
public:
	clear_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		Core::ClearLog();
	}
private:

};

class culling_toggle_functor : public TOW_Functor
{
public:
	culling_toggle_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		
	}
private:

};

class exec_functor : public TOW_Functor
{
public:
	exec_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		std::string s = parameters[0].as_if_string();
		std::ifstream f(s.c_str());
		if(f.fail()) 
		{
			Core::WriteLog("Script file not found!\n", 1);
			return;
		}
		std::string l;
		char ch;
		while(!f.eof())
		{
			f.read(&ch, 1);
			if(ch == '\n')
			{
				c->exec_command(l);
				l.erase();
			}
			else
			{
				l += ch;
			}
		}
		if(l != "") c->exec_command(l);
		f.close();
	}
};

class update_object_functor : public TOW_Functor
{
public:
	update_object_functor(TOW_World* world)
	{
		m_world = world;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		m_world->UpdateGameObject(parameters[0].as_if_string());
	}
private:
	TOW_World* m_world;
};

class game_speed_functor : public TOW_Functor
{
public:
	game_speed_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		Core::SetGameSpeed(parameters[0].as_if_float());
	}
private:
	
};

class mem_stat_functor : public TOW_Functor
{
public:
	mem_stat_functor()
	{
		has_been_called = false;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		MEMORYSTATUS m;
		GlobalMemoryStatus(&m);
		std::stringstream ss;
		ss << "Memory Status: " << m.dwMemoryLoad << "% used. ";
		if(has_been_called)
			ss << "Bytes free: " << m.dwAvailPhys << std::endl;
		else
			ss << "Total Bytes: " << m.dwTotalPhys << std::endl;
		Core::WriteLog(ss.str().c_str(), 1);
		has_been_called = true;
	}
private:
	MEMORYSTATUS mem_stat;
	bool has_been_called;
};

class bind_functor : public TOW_Functor
{
public:
	bind_functor(UICommandMap* m)
	{
		m_map = m;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		std::string s;
		if(parameters.size() == 1) //get binding...
		{
			s = m_map->Command(m_map->StringToKeyConst(parameters[0].as_if_string()));
			if(s == "fail")
			{
				Core::WriteLog("This key is unbound!\n", 0);
				return;
			}
			Core::WriteLog(s.c_str(), 0);
			Core::WriteLog("\n", 0);
		}
		if(parameters.size() == 2) // set binding...
		{
			m_map->Bind(m_map->StringToKeyConst(parameters[0].as_if_string()), parameters[1].as_if_string());
		}
	}
private:
	UICommandMap* m_map;
};

class console_s_functor : public TOW_Functor
{
public:
	console_s_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		UIConsoleWnd* uw;
		uw = (UIConsoleWnd*)&singleton<UserInterface*>::instance()->Windows().Window(moniker("console"));
		if(uw)
		{
			window_message m;
			m.message = CONMSG_EXPAND;			
			uw->send_message(m);
			singleton<UserInterface*>::instance()->Windows().ActiveWindow(uw);
		}
	}
private:
	
};

class seq_functor : public TOW_Functor
{
public:
	seq_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		string_tokenizer<> s(parameters[0].as_if_string(), std::string(";"));
		for(int i = 0; i < s.size(); ++i)
			c->exec_command(s.token(i));
	}
private:
	
};

class start_sel_functor : public TOW_Functor
{
public:
	start_sel_functor(IDevice* device, TOW_Selector* sel, CCamera* cam, TOW_Map* map, TOW_SelectBox** sel_box)
	{
		m_device = device;
		m_sel = sel;
		m_camera = cam;
		m_map = map;
		m_sel_box = sel_box;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters.size() == 0)
		{
			ray r;
			POINT p;
			singleton<UserInterface*>::instance()->Input().GetMousePos(&p);
			VECTOR v(float(p.x), float(p.y), 0);
			VECTOR o;
			
			
			m_camera->compute_ray(&r, &v);
			bool b = m_map->HeightMap()->RayCast(&o, &r.orig, &r.dir, 1.0f);
			if(b)	
			{
				float x, z;
				x = o.x;
				z = o.z;
				m_sel->Start(VECTOR(x, 0.0f, z));
				(*m_sel_box) = new TOW_SelectBox;
				(*m_sel_box)->SetWidth(0.5f);
				(*m_sel_box)->SetColor(0xff00ff00);
				(*m_sel_box)->SetStartPoint(o);
				singleton<TOW_ObjectManager*>::instance()->RegisterObject((*m_sel_box));
			}	
		}
		else
		{
			float x, z;
			x = parameters[0].as_if_float();
			z = parameters[1].as_if_float();
			m_sel->Start(VECTOR(x, 0.0f, z));
		}
	}
private:
	TOW_Selector* m_sel;
	CCamera* m_camera;
	TOW_Map* m_map;
	TOW_SelectBox** m_sel_box;
};

class end_sel_functor : public TOW_Functor
{
public:
	end_sel_functor(IDevice* device, TOW_Selector* sel, CCamera* cam, TOW_Map* map, TOW_World* world, TOW_SelectBox** sel_box)
	{
		m_device = device;
		m_sel = sel;
		m_camera = cam;
		m_map = map;
		m_world = world;
		m_sel_box = sel_box;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters.size() == 0)
		{
			ray r;
			POINT p;
			singleton<UserInterface*>::instance()->Input().GetMousePos(&p);
			VECTOR v(float(p.x), float(p.y), 0);
			VECTOR o;
			
			m_camera->compute_ray(&r, &v);
			bool b = m_map->HeightMap()->RayCast(&o, &r.orig, &r.dir, 1.0f);
			if(b)	
			{
				float x, z;
				x = o.x;
				z = o.z;
				m_sel->End(VECTOR(x, 0.0f, z));
				m_sel->Compute();
				if((*m_sel_box))
					singleton<TOW_ObjectManager*>::instance()->Destroy((*m_sel_box));
				(*m_sel_box) = NULL;
			}
			if(m_sel->Count() == 0)
			{
				moniker m = m_world->HitTest(&r);
				TOW_GameObject* go = (TOW_GameObject*)singleton<TOW_ObjectManager*>::instance()->GetObject(m);
				if(go)
				{
					int se = go->property(std::string("selectable")).as_if_int();
					if(se)
						m_sel->AddObject(m);
				}
			}
		}
		else
		{
			float x, z;
			x = parameters[0].as_if_float();
			z = parameters[1].as_if_float();
			m_sel->End(VECTOR(x, 0.0f, z));
			m_sel->Compute();
		}
	}
private:
	TOW_Selector* m_sel;
	CCamera* m_camera;
	TOW_Map* m_map;
	TOW_World* m_world;
	TOW_SelectBox** m_sel_box;
};

class move_functor : public TOW_Functor
{
public:
	move_functor(TOW_CameraController* camera_cont)
	{
		m_camera_cont = camera_cont;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		std::string str(parameters[0].as_if_string());
		if(str.size() != 3)
		{
			return;
		}
		VECTOR v = m_camera_cont->Dir();
		if(str[0] == '+')
		{
			if(str[1] == 'x')
			{
				if(str[2] == '+')
				{
					v.x += 1.0f;
				}
				else
				{
					v.x -= 1.0f;
				}
			}
			else
			{
				if(str[2] == '+')
				{
					v.z += 1.0f;
				}
				else
				{
					v.z -= 1.0f;
				}
			}
		}
		else
		{
			if(str[1] == 'x')
			{
				if(str[2] == '+')
				{
					v.x -= 1.0f;
				}
				else
				{
					v.x += 1.0f;
				}
			}
			else
			{
				if(str[2] == '+')
				{
					v.z -= 1.0f;
				}
				else
				{
					v.z += 1.0f;
				}
			}
		}
		m_camera_cont->Dir(v);
	}	
private:
	TOW_CameraController* m_camera_cont;
};

class z_move_functor : public TOW_Functor
{
public:
	z_move_functor(VECTOR* dir)
	{
		m_dir = dir;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters[0].as_if_string() == "+")
		{
			m_dir->z += 1.0f;
		}
		else
		{
			m_dir->z += -1.0f;
		}
	}
private:
	VECTOR* m_dir;
};

class x_stop_functor : public TOW_Functor
{
public:
	x_stop_functor(VECTOR* dir)
	{
		m_dir = dir;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters[0].as_if_string() == "+")
		{
			m_dir->x += -1.0f;
		}
		else
		{
			m_dir->x += 1.0f;
		}
		
	}
private:
	VECTOR* m_dir;
};

class z_stop_functor : public TOW_Functor
{
public:
	z_stop_functor(VECTOR* dir)
	{
		m_dir = dir;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters[0].as_if_string() == "+")
		{
			m_dir->z += -1.0f;
		}
		else
		{
			m_dir->z += 1.0f;
		}
	}
private:
	VECTOR* m_dir;
};



class context_functor : public TOW_Functor
{
public:
	context_functor(IDevice* device, TOW_Selector* sel, CCamera* cam, TOW_Map* map, TOW_World* world)
	{
		m_device = device;
		m_sel = sel;
		m_camera = cam;
		m_map = map;
		m_world = world;
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		ray r;
		POINT p;
		singleton<UserInterface*>::instance()->Input().GetMousePos(&p);
		VECTOR v(float(p.x), float(p.y), 0);
		VECTOR o;
		VECTOR delta;
		float max_size = 0.0f;
		std::stringstream ss;
		if(m_sel->Count() == 0)
			return;
		m_camera->compute_ray(&r, &v);
		PLANE pl;
		pl.a = pl.c = pl.d = 0.0f;
		pl.b = 1.0f;
		TOW_GameObject* go;
		go = (TOW_GameObject*)singleton<TOW_ObjectManager*>::instance()->GetObject(m_world->HitTest(&r));
		if(go)
		{
			if(m_sel->Count() == 0)
				return;
			ss << "attack " << go->name()();
			m_sel->DispatchOrder(ss.str());
			return;
	
		}
		bool b = r.intersect_plane(&o, &pl); //m_map->HeightMap()->RayCast(&o, &r.orig, &r.dir, 1.0f);
		if(m_sel->Count() == 1)
		{
			if(b)	
			{
				float x, z;
				x = o.x;
				z = o.z;
				ss << "goto " << x << ' ' << z;
				m_sel->DispatchOrder(ss.str());		
			}
		}
		else
		{
			
			for(int i = 0; i < m_sel->Count(); ++i)
			{
				float a = m_world->GetObject(m_sel->Object(i))->property(std::string("bound_sphere_radius")).as_if_float();
				if(a > max_size)
					max_size = a*2;
			}
			
			delta = m_world->GetObject(m_sel->Object(0))->pos - o;
			float m;
			if(delta.z == 0)
			{
				delta.x = 0;
				delta.z = 1;
				delta.y = 0;
			}
			else
			{
				if(delta.x == 0)
				{
					m = 0;
				}
				else
				{
					m = delta.z / delta.x;
				}
				delta.x = m;
				delta.z = 1;
				delta.y = 0;
				VectorNormalize(&delta, &delta);
			}
				
			int sc = sqrt(m_sel->Count());
			float offset = float(sc) - float(sc)/2.0f;
			int co=0;
			for(i = 0; i < m_sel->Count(); ++i)
			{
				float x, z;
				x = o.x + (delta.x * max_size * float(i%sc) + offset);
				z = o.z + (delta.z * max_size * float(co));
				
				if(i%sc == 0)
					++co;
				ss << "goto " << x << ' ' << z;
				m_world->GetObject(m_sel->Object(i))->DispatchOrder(ss.str());	
				ss.str(std::string(""));
			}
		}
	}
private:
	TOW_Selector* m_sel;
	CCamera* m_camera;
	TOW_Map* m_map;
	TOW_World* m_world;
	
};

class main_menu_functor : public TOW_Functor
{
public:
	main_menu_functor(int* mode)
	{
		m_mode = mode;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		*m_mode = 1;
	}
private:
	int* m_mode;
};

class connect_functor : public TOW_Functor
{
public:
	connect_functor(tow_networking* net, TOW_Player* player)
	{
		m_net = net;
		m_player = player;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		int port;
		string_tokenizer<> s(parameters[0].as_if_string(), std::string(":"));
		if(s.size() > 1)
		{
			port = atoi(s.token(1).c_str());
		}
		else
		{
			port = 5600;
		}
		if(parameters.size() == 1)
		{
			m_net->Logon((char*)s.token(0).c_str(), port, 5000, (char*)m_player->name.c_str(), "00", "1.0");
		}
	}
private:
	tow_networking* m_net;
	TOW_Player* m_player;
};

class name_functor : public TOW_Functor
{
public:
	name_functor(TOW_Player* player)
	{
		m_player = player;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		m_player->name = parameters[0].as_if_string();
	}
private:
	TOW_Player* m_player;
};


class say_functor : public TOW_Functor
{
public:
	say_functor(tow_networking* net)
	{
		m_net = net;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(m_net->GetConnected(DN_TCP))
		{
			m_net->Message((char*)parameters[0].as_if_string().c_str());
		}
		else
		{
			Core::WriteLog("Not Connected to a game server!\n", 0);
		}
	}
private:
	tow_networking* m_net;
};

class whisper_functor : public TOW_Functor
{
public:
	whisper_functor(tow_networking* net)
	{
		m_net = net;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(m_net->GetConnected(DN_TCP))
		{
			m_net->Whisper((char*)parameters[0].as_if_string().c_str(), (char*)parameters[0].as_if_string().c_str());
		}
		else
		{
			Core::WriteLog("Not Connected to a game server!\n", 0);
		}
	}
private:
	tow_networking* m_net;
};

class join_functor : public TOW_Functor
{
public:
	join_functor(tow_networking* net)
	{
		m_net = net;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(m_net->GetConnected(DN_TCP))
		{
			m_net->Join((char*)parameters[0].as_if_string().c_str());
		}
		else
		{
			Core::WriteLog("Not Connected to a game server!\n", 0);
		}
	}
private:
	tow_networking* m_net;
};

class map_functor : public TOW_Functor
{
public:
	map_functor(TOW_Map** map)
	{
		m_map = map;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		static map_load_thread mlt;
		std::string name(parameters[0].as_if_string());
		
		
		string_tokenizer<> s(name, std::string("."));
		if(s.token(s.size()-1) != "twm")
		{
			name += ".twm";
		}
		FILE* f = fopen(name.c_str(), "r+b");
		if(!f)
		{
			Core::WriteLog("Map does not exist!\n", 0);
			return;
		}
		fclose(f);
		if(!(*m_map))
		{
			(*m_map) = new TOW_Map;
			
		}
		else
		{
			(*m_map)->Destroy();
			(*m_map)->ClearObjects(true);
		}
		singleton<TOW_Map*> smap(*m_map);
		(*m_map)->SetCamera(singleton<CCamera*>::instance());
		/*(*m_map)->Load((char*)name.c_str());
		(*m_map)->Create(0, 0, 0.0f);*/
		mlt = map_load_thread(*m_map, name);
		mlt.begin(false, false);
			
	}
private:
	TOW_Map** m_map;
};

class alias_functor : public TOW_Functor
{
public:
	alias_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		c->alias(parameters[0].as_if_string(), parameters[1].as_if_string());	
	}
private:
	
};

class developer_functor : public TOW_Functor
{
public:
	developer_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		long l = parameters[0].as_if_long();
		Core::SetDebugLogLevel(l);
	}
private:
	
};

class cmd_list_functor : public TOW_Functor
{
public:
	cmd_list_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		std::vector<std::string> list;
		if(parameters.size() == 0)
		{
			c->command_list(list);
			std::sort(list.begin(), list.end());
			for(std::vector<std::string>::iterator i = list.begin(); i != list.end(); ++i)
			{
				Core::WriteLog((*i).c_str(), 0);
				Core::WriteLog("\n", 0);
			}
			return;
		}
	}
private:
	
};

class shadows_functor : public TOW_Functor
{
public:
	shadows_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		singleton<TOW_Map*>::instance()->ToggleShadows();
	}
private:
	
};

#endif