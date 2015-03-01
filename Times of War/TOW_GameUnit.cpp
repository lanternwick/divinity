#include "TOW_GameUnit.h"
#include "..\Common\singleton.h"


TOW_GameUnit::TOW_GameUnit()
{
	m_state = NULL;
	
}

TOW_GameUnit::TOW_GameUnit(const char* id)
{
	m_id() = id;
	m_state = NULL;
	m_selind = NULL;
	
}

TOW_GameUnit::~TOW_GameUnit()
{

}

RESULT TOW_GameUnit::Think()
{
	RESULT r;
	
	int hp = property(std::string("current_HP")).as_if_int();
	if(hp <= 0)
	{
		ClearState();
		Deselect();
		return DESTROY;
	}
	if(m_state)
	{
		r = m_state->Think();
		if(r == END_STATE)
		{
			ClearState();
		}
	}
	return OK;
}

moniker TOW_GameUnit::GetMesh()
{
	return m_mesh;
}

void TOW_GameUnit::SetMesh(moniker& mesh)
{
	m_mesh = mesh;
}

moniker TOW_GameUnit::GetSkin()
{
	return m_skin;
}

void TOW_GameUnit::SetSkin(moniker& skin)
{
	m_skin = skin;
}

void TOW_GameUnit::Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, std::vector<variant>& params)
{
	if(function_name == "set_skin")
	{
		m_skin() = params[0].as_if_string();
	}
	if(function_name == "get_info")
	{
		log_stream<1> ls;
		properties.output(ls);
	}
	if(function_name == "set_pos")
	{
		pos.x = params[0].as_if_float();
		pos.z = params[1].as_if_float();	
	}
	if(function_name == "get_pos")
	{
		log_stream<0> ls;
		ls << "(" << pos.x << ", " << pos.z << ")\n";
	}
	if(function_name == "goto")
	{
		moving_state* ms = new moving_state(m_world);
		std::stringstream ss;
		ss << name()() << ' ' << pos.x << ' ' << pos.z << ' ' << params[1].as_if_float() << ' ' << params[2].as_if_float() << ' ' << params[0].as_if_float();
		ms->set_state_info(ss.str());
		m_state = ms;
		ms->StateEnter();
	}


}

void TOW_GameUnit::DispatchOrder(std::string order)
{
	string_tokenizer<> s(order, std::string(" "));
	std::string order_name(s.token(0));
	if(order_name == "goto")
	{
		ClearState();
		moving_state* ms = new moving_state(m_world);
		std::stringstream ss;
		ss << name()() << ' ' << pos.x << ' ' << pos.z << ' ' << s.token(1) << ' ' << s.token(2) << ' ' << properties.property(std::string("speed")).as_if_float();
		ms->set_state_info(ss.str());
		m_state = ms;
		ms->StateEnter();
	}
	if(order_name == "attack")
	{
		ClearState();
		attack_state* as = new attack_state(m_world);
		std::stringstream ss;
		ss << name()() << ' ' << s.token(1) << ' ' << property(std::string("range")).as_if_float()
			<< ' ' << property(std::string("rate_of_fire")).as_if_float();
		as->set_state_info(ss.str());
		m_state = as;
		as->StateEnter();
	}
	if(order_name == "selected")
	{
		Select();
	}
	if(order_name == "deselected")
	{
		Deselect();
	}

}

void TOW_GameUnit::Message(std::string& message)
{
	string_tokenizer<> s(message, std::string(" "));
	if(!property(std::string("selectable")).as_if_int())
		return;
	if(s.token(0) == "damage_taken")
	{
		if(m_world->GetObject(moniker((char*)s.token(1).c_str())))
		{
			std::stringstream ss;
			ss << "attack " << s.token(1);
			DispatchOrder(ss.str());
		}
	}
}

void TOW_GameUnit::Select()
{
	selected = true;
	m_selind = new TOW_SelectIndicator;
	m_selind->Create(m_device, m_factory);
	m_selind->Color(m_owner->color);
	m_selind->SetGameObject(this);
	singleton<TOW_ObjectManager*>::instance()->RegisterObject(m_selind);
}

void TOW_GameUnit::Deselect()
{
	selected = false;
	if(m_selind)
	{
		m_selind->SetGameObject(NULL);
		singleton<TOW_ObjectManager*>::instance()->Destroy(m_selind);
	}
	m_selind = NULL;
}

bool TOW_GameUnit::Destroy(float t)
{
	dest_t = t;
	property(std::string("mesh")) = std::string("snow_corpse");
	Deselect();
	ClearState();
	return true;
}

bool TOW_GameUnit::DestroyTick()
{
	if(Core::GetTime() > dest_t + 2.5f)
		return false;
	else 
		return true;
}