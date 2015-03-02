#include "TOW_InputHandler.h"
#include "..\DivCore\Core.h"
#include "..\Common\Singleton.h"
#include "..\DivUI\UICommandMap.h"
#include "..\console\console.h"
#include "lua_glue.h"
#include "..\DivUI\KeyConstants.h"
#include "..\DivUI\UIConsoleWnd.h"
#include "..\DivUI\UserInterface.h"

TOW_InputHandler::TOW_InputHandler()
{
	shift = false;
}

TOW_InputHandler::~TOW_InputHandler()
{

}

bool TOW_InputHandler::Keyboard_KeyDown(char key)
{
	

	UIWindow* uw;
	UserInterface* ui = singleton<UserInterface*>::instance();
	shift = (ui->Input().GetKey(DIV_KEY_RSHIFT) | ui->Input().GetKey(DIV_KEY_LSHIFT));
	uw = &ui->Windows().Window(moniker("console"));
	std::string command;
	window_message m;
	std::string s;
	if(key == DIV_KEY_RSHIFT)
		return true;
	if(key == DIV_KEY_LSHIFT)
		return true;
	if(!ui->Windows().ActiveWindow())
	{
		command = singleton<UICommandMap*>::instance()->Command(key);
		if(command != "fail")
		{
			singleton<console*>::instance()->exec_command(command);
		}
	}
	else
	{
		switch((unsigned char)key)
		{
		case DIV_KEY_RETURN:
			
			m.message = CONMSG_GETBUFFER;
			uw->send_message(m);
			s = m.string_ret;
			Core::WriteLog(s.c_str(), 0);
			Core::WriteLog("\n", 0);
			//try
			{
				m.message = CONMSG_SAVECMD;
				uw->send_message(m);
				m.message = CONMSG_CLEARBUFFER;
				uw->send_message(m);
				if(s[0] == '/' && (s[1] == 'l' || s[1] == 'L'))
				{
					lua_lua_dobuffer(s.c_str()+3, s.size()-3);
				}
				else
				{
					singleton<console*>::instance()->exec_command(s);
				}
			}
			//catch(...)
			{
			//	Core::WriteLog("Exception occured while executing console command\n", 1);
			}
			break;

		case DIV_KEY_PGUP:
			// scroll log up
			m.message = CONMSG_INCOFFSET;
			uw->send_message(m);
			//uw->SetOffset(uw->GetOffset()+uw->GetNumLinesAtState(uw->GetState()));
			break;
		case DIV_KEY_PGDN:
			//scroll log down
			m.message = CONMSG_DECOFFSET;
			uw->send_message(m);
			//uw->SetOffset(uw->GetOffset()-uw->GetNumLinesAtState(uw->GetState()));
			break;
		case DIV_KEY_UP:
			//next hist command
			m.message = CONMSG_NEXTHIST;
			uw->send_message(m);
			//uw->NextCmd();
			break;
		case DIV_KEY_DOWN:
			// prev hist command
			m.message = CONMSG_PREVHIST;
			uw->send_message(m);
			//uw->PrevCmd();
			break;
		default:
			// post the key to the buffer
			m.message = CONMSG_POSTKEY;
			m.ulong_in[0] = ui->Input().KeyToChar(key, shift);
			uw->send_message(m);
			//uw->AddKeyToBuffer(singleton<UserInterface*>::instance()->KeyToChar(key, shift));
			break;
			
		}
	}
	
	return true;
}

bool TOW_InputHandler::Keyboard_KeyUp(char key)
{
	std::string command;
	if(!singleton<UserInterface*>::instance()->Windows().ActiveWindow())
	{
		command = singleton<UICommandMap*>::instance()->Command((unsigned long)key);
		if(command != "fail")
		{
			if(command[0] == '+')
			{
				command[0] = '-';
				singleton<console*>::instance()->exec_command(command);
			}		
		}

	}
	return true;
}

bool TOW_InputHandler::Mouse_ButtonUp(int button)
{
	int b_map;
	std::string command;
	if(button == 0) b_map = DIV_KEY_MOUSE1_UP;
	if(button == 1) b_map = DIV_KEY_MOUSE2_UP;
	if(button == 2) b_map = DIV_KEY_MOUSE3_UP;
	if(button == 3) b_map = DIV_KEY_MOUSE4_UP;
	command = singleton<UICommandMap*>::instance()->Command(b_map);
	if(command != "fail")
	{
		singleton<console*>::instance()->exec_command(command);
	}

	/*CCamera c;
	m_towm->currScene->GetCamera(&c);
	
	ray r;
	POINT p;
	m_towm->ui->GetMousePos(&p);
	VECTOR v(float(p.x), float(p.y), 0);
	VECTOR o;
	
	
	m_towm->myDevice->ComputeRay(&r, &v, &c);
	bool b = m_towm->map->HeightMap()->RayCast(&o, &r.orig, &r.dir, 1.0f);
	std::stringstream ss;
	ss << "update_object(\"tow move abs " << o.x << " " << o.z << "\")";
	Core::WriteLog(ss.str().c_str(), 1);
	Core::WriteLog("\n", 1);
	m_towm->tow_console.exec_command(ss.str());*/
	return true;
}

bool TOW_InputHandler::Mouse_ButtonDown(int button)
{
	int b_map;
	std::string command;
	if(button == 0) b_map = DIV_KEY_MOUSE1_DOWN;
	if(button == 1) b_map = DIV_KEY_MOUSE2_DOWN;
	if(button == 2) b_map = DIV_KEY_MOUSE3_DOWN;
	if(button == 3) b_map = DIV_KEY_MOUSE4_DOWN;
	command = singleton<UICommandMap*>::instance()->Command(b_map);
	if(command != "fail")
	{
		singleton<console*>::instance()->exec_command(command);
	}
	return true;
}

bool TOW_InputHandler::Mouse_Move(POINT p, int mb1, int mb2, int mb3, int m4)
{
	//CCamera c;
	/*if(m_towm->m_mode == MENU)
		return true;
	m_towm->currScene->GetCamera(&c);
	
	ray r;
	VECTOR v(float(p.x), float(p.y), 0);
	VECTOR o;
	
	
	singleton<IDevice*>::instance()->ComputeRay(&r, &v, &c);

	//myDevice->Unproject(&s, &v, 1.0f);
	//myDevice->Unproject(&e, &v2, 1.0f);
	
	//bool b = map->HeightMap()->RayCast(&o, &s, &(e - s), 1.0f);

	//if(RayIntersectSphere(&o, &VECTOR(0.0f, map->HeightMap()->GetHeight(0.0f, 0.0f), 0.0f), 1.0f, &s, &(e-s)))
	TOW_GameObject* go;
	go = (TOW_GameObject*)singleton<TOW_ObjectManager*>::instance()->GetObject(m_towm->world->HitTest(&r));*/
	/*if(go)
	{
		m_towm->ui->SetCursorInfo(go->property(std::string("name")).as_if_string());
	}
	else
	{
		m_towm->ui->SetCursorInfo(std::string("TOW"));
	}*//*
	if(r.intersect_sphere(&o, &VECTOR(0, m_towm->map->HeightMap()->GetHeight(0.0f, 0.0f), 0), 2.0f)) //LineIntersectSphere(&o, &VECTOR(0, map->HeightMap()->GetHeight(0.0f, 0.0f), 0), 2.0f, &s, &e))
	{
		m_towm->ui->SetCursorInfo(std::string("Tower"));
	}
	else
	{
		m_towm->ui->SetCursorInfo(std::string("TOW"));
	}*/
	return true;
}

bool TOW_InputHandler::Mouse_Scroll(long scroll)
{
	//m_towm->yAdd+=1.0f*(float)scroll/120;
	//if(m_towm->yAdd > 20.0f) m_towm->yAdd = 20.0f;
	//if(m_towm->yAdd < 8.0f) m_towm->yAdd = 8.0f;
	std::string command;
	int b_map = scroll > 0 ? DIV_KEY_MOUSESCROLL_UP : DIV_KEY_MOUSESCROLL_DOWN;
	command = singleton<UICommandMap*>::instance()->Command(b_map);
	if(command != "fail")
	{
		singleton<console*>::instance()->exec_command(command);
	}
	return true;
}

