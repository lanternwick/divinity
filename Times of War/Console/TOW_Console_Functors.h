#ifndef TOW_CONSOLE_FUNCTORS_H
#define TOW_CONSOLE_FUNCTORS_H

#include "TOW_Functor.h"
#include "TOW_World.h"
#include <fstream>
#include "..\DivUserInterface\UICommandMap.h"
#include "..\TOW_Selector.h"
#include "..\TOW_SelectBox.h"
#include "..\TOW_Player.h"
#include "..\tow_networking.h"
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
	quit_functor()
	{
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		*m_running = false;
		if(parameters.size() == 1)
		{
			Core::WriteLog("Quitting!\n", 0);
		}
	}
	void set_running(bool* running)
	{
		m_running = running;
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
		UIConsoleWnd* uw;	
		uw = (UIConsoleWnd*)Core::GetMediaObject(moniker("console"));
		uw->SetPrompt(parameters[0].as_if_string());
		
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
			s = m_map->Command(UserInterface::Instance()->StringToKeyConst(parameters[0].as_if_string()));
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
			m_map->Bind(UserInterface::Instance()->StringToKeyConst(parameters[0].as_if_string()), parameters[1].as_if_string());
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
		uw = (UIConsoleWnd*)Core::GetMediaObject(moniker("console"));
		if(uw) 
			uw->SetState(uw->GetState() ? 0 : 2);
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
	start_sel_functor(IDevice* device, TOW_Selector* sel, CScene* s, TOW_Map* map, TOW_SelectBox** sel_box)
	{
		m_device = device;
		m_sel = sel;
		m_scene = s;
		m_map = map;
		m_sel_box = sel_box;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters.size() == 0)
		{
			CCamera c;
			m_scene->GetCamera(&c);
			ray r;
			POINT p;
			UserInterface::Instance()->GetMousePos(&p);
			VECTOR v(float(p.x), float(p.y), 0);
			VECTOR o;
			
			
			m_device->ComputeRay(&r, &v, &c);
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
	CScene* m_scene;
	TOW_Map* m_map;
	TOW_SelectBox** m_sel_box;
};

class end_sel_functor : public TOW_Functor
{
public:
	end_sel_functor(IDevice* device, TOW_Selector* sel, CScene* s, TOW_Map* map, TOW_World* world, TOW_SelectBox** sel_box)
	{
		m_device = device;
		m_sel = sel;
		m_scene = s;
		m_map = map;
		m_world = world;
		m_sel_box = sel_box;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters.size() == 0)
		{
			CCamera c;
			m_scene->GetCamera(&c);
			ray r;
			POINT p;
			UserInterface::Instance()->GetMousePos(&p);
			VECTOR v(float(p.x), float(p.y), 0);
			VECTOR o;
			
			m_device->ComputeRay(&r, &v, &c);
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
	CScene* m_scene;
	TOW_Map* m_map;
	TOW_World* m_world;
	TOW_SelectBox** m_sel_box;
};

class x_move_functor : public TOW_Functor
{
public:
	x_move_functor(VECTOR* dir)
	{
		m_dir = dir;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		if(parameters[0].as_if_string() == "+")
		{
			m_dir->x += 1.0f;
		}
		else
		{
			m_dir->x += -1.0f;
		}
	}
private:
	VECTOR* m_dir;
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
	context_functor(IDevice* device, TOW_Selector* sel, CScene* s, TOW_Map* map, TOW_World* world)
	{
		m_device = device;
		m_sel = sel;
		m_scene = s;
		m_map = map;
		m_world = world;
		
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		
		CCamera ca;
		m_scene->GetCamera(&ca);
		ray r;
		POINT p;
		UserInterface::Instance()->GetMousePos(&p);
		VECTOR v(float(p.x), float(p.y), 0);
		VECTOR o;
		VECTOR delta;
		float max_size = 0.0f;
		std::stringstream ss;
		if(m_sel->Count() == 0)
			return;
		m_device->ComputeRay(&r, &v, &ca);
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
	CScene* m_scene;
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
	map_functor(TOW_Map* map)
	{
		m_map = map;
	}
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
	{
		std::string name(parameters[0].as_if_string());
		m_map->Destroy();
		string_tokenizer<> s(name, std::string("."));
		if(s.token(s.size()-1) != "map")
		{
			name += ".map";
		}
		m_map->ClearObjects(true);
		
		m_map->Load((char*)name.c_str());
		m_map->Create(0, 0, 0.0f);
	}
private:
	TOW_Map* m_map;
};

#endif