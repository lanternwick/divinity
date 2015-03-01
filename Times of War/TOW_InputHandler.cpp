
TOW_InputHandler::TOW_InputHandler()
{
	shift = false;
}

TOW_InputHandler::~TOW_InputHandler()
{

}

bool TOW_InputHandler::Keyboard_KeyDown(char key)
{
	

	UIConsoleWnd* uw;
	shift = (m_towm->ui->GetKeyState(DIV_KEY_RSHIFT) | m_towm->ui->GetKeyState(DIV_KEY_LSHIFT));
	uw = (UIConsoleWnd*)Core::GetMediaObject(moniker("console"));
	std::string command;
	/*if(key == DIV_KEY_GRAVE)
	{
		if(uw) 
			uw->SetState((uw->GetState() ? 0 : 2));
	}*/
	
	if(key == DIV_KEY_RSHIFT)
		return true;
	if(key == DIV_KEY_LSHIFT)
		return true;
	if(uw->GetState() < 2)
	{
		command = m_towm->m_commandmap.Command(key);
		if(command != "fail")
		{
			m_towm->tow_console.exec_command(command);
		}
		/*if(key == DIV_KEY_ESCAPE)
			m_towm->running = false;
		if(key == DIV_KEY_W)
			//myDevice->SetRenderState(RS_FILLMODE, FILL_WIREFRAME);
			m_towm->tow_console.exec_command(std::string("wireframe()"));
		if(key == DIV_KEY_S)
			//myDevice->SetRenderState(RS_FILLMODE, FILL_SOLID);
			m_towm->tow_console.exec_command(std::string("solid()"));*/
		/*if(key == DIV_KEY_T)
		{
			te_mode = true;
			return;
		}*/

	}
	else
	{
		if(key == DIV_KEY_RETURN)
		{
			Core::WriteLog(uw->GetBuffer().c_str(), 0);
			Core::WriteLog("\n", 0);
			//try
			{

				m_towm->tow_console.exec_command(uw->GetBuffer());
			}
			//catch(...)
			{
			//	Core::WriteLog("Exception occured while executing console command\n", 1);
			}
			
			uw->ClearBuffer();
			
			//Core::WriteLog(m_towm->console_prompt.c_str());
		}
		else
		{
			uw->AddKeyToBuffer(m_towm->ui->KeyToChar(key, shift));
			/*if(c[0] == '\b')
			{
				if(strlen(Core::ReadLog(1)) > m_towm->console_prompt.size())
				{
					Core::WriteLog(c);
				}
			}
			else
			{
				Core::WriteLog(c);
			}*/
			
		}
	}
	
	return true;
}

bool TOW_InputHandler::Keyboard_KeyUp(char key)
{
	UIConsoleWnd* uw;
	
	uw = (UIConsoleWnd*)Core::GetMediaObject(moniker("console"));
	std::string command;
	if(uw->GetState() < 2)
	{
		command = m_towm->m_commandmap.Command((unsigned long)key);
		if(command != "fail")
		{
			if(command[0] == '+')
			{
				command[0] = '-';
				m_towm->tow_console.exec_command(command);
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
	command = m_towm->m_commandmap.Command(b_map);
	if(command != "fail")
	{
		m_towm->tow_console.exec_command(command);
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
	command = m_towm->m_commandmap.Command(b_map);
	if(command != "fail")
	{
		m_towm->tow_console.exec_command(command);
	}
	return true;
}

bool TOW_InputHandler::Mouse_Move(POINT p, int mb1, int mb2, int mb3, int m4)
{
	CCamera c;
	m_towm->currScene->GetCamera(&c);
	
	ray r;
	VECTOR v(float(p.x), float(p.y), 0);
	VECTOR o;
	
	
	m_towm->myDevice->ComputeRay(&r, &v, &c);

	//myDevice->Unproject(&s, &v, 1.0f);
	//myDevice->Unproject(&e, &v2, 1.0f);
	
	//bool b = map->HeightMap()->RayCast(&o, &s, &(e - s), 1.0f);

	//if(RayIntersectSphere(&o, &VECTOR(0.0f, map->HeightMap()->GetHeight(0.0f, 0.0f), 0.0f), 1.0f, &s, &(e-s)))
	TOW_GameObject* go;
	go = m_towm->world->HitTest(&r);
	if(go)
	{
		m_towm->ui->SetCursorInfo(go->property(std::string("name")).as_if_string());
	}
	else
	{
		m_towm->ui->SetCursorInfo(std::string("TOW"));
	}/*
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
	command = m_towm->m_commandmap.Command(b_map);
	if(command != "fail")
	{
		m_towm->tow_console.exec_command(command);
	}
	return true;
}

void TOW_InputHandler::SetTOWMod(TimesOfWar_Mod* towm)
{
	m_towm = towm;
}