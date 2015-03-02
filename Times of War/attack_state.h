#ifndef ATTACK_STATE_H
#define ATTACK_STATE_H

#pragma warning(disable : 4786)

#include "game_state.h"
#include "moving_state.h"
#include "TOW_World.h"
#include "TOW_Projectile.h"
#include <sstream>

class attack_state : public game_state
{
public:
	attack_state(){}
	attack_state(TOW_World* world){m_world = world;moving = false;ms = NULL;}
	virtual ~attack_state(){if(ms)delete ms;ms=NULL;};
	virtual RESULT Think()
	{
		TOW_GameObject* tgo;
		TOW_GameObject* ogo;
		tgo = m_world->GetObject(moniker((char*)target.c_str()));
		ogo = m_world->GetObject(moniker((char*)obj.c_str()));
		if(tgo == NULL)
			return END_STATE;
		if(ogo == NULL)
			return END_STATE;
		float dist = sqrt(
			((ogo->pos.x - tgo->pos.x)*(ogo->pos.x - tgo->pos.x))+
			((ogo->pos.z - tgo->pos.z)*(ogo->pos.z - tgo->pos.z)));
		next_attack -= Core::GetTimeDelta();
		RESULT r;
		if(moving)
		{
			if(dist < range)
			{
				moving = false;
				delete ms;
				ms = NULL;
			}
			else
			{
				r = ms->Think();
			}
		}
		else
		{
			if(dist > range)
			{
				moving = true;
				ms = new moving_state(m_world);
				std::stringstream ss;
				ss << obj << ' ' << ogo->pos.x << ' ' << ogo->pos.z << ' ' <<
					tgo->pos.x << ' ' << tgo->pos.z << ' ' << ogo->property(std::string("speed")).as_if_float();
				ms->set_state_info(ss.str());
				ms->Think();
			}
			else
			{
				if(next_attack > 0.0f)
					return OK;
				TOW_Projectile* p = new TOW_Projectile;
				p->SetWorld(m_world);
				p->SetOwner(ogo->name()());
				p->SetTarget(tgo->name()());
				p->SetStart(ogo->pos);
				p->SetEnd(tgo->pos);
				
				std::stringstream ss;
				ss << ogo->name()() << " projectile";
				m_world->CreateObject(moniker((char*)ogo->property(std::string("projectile_type")).as_if_string().c_str()), moniker((char*)ss.str().c_str()), p);
				p->SetVelocity(p->property(std::string("velocity")).as_if_float());
				next_attack = 1.0f / rof;
			}
		}
		return OK;
	}
	virtual void set_state_info(std::string& info)
	{
		m_info = info;
		string_tokenizer<> s(info, std::string(" "));
		obj = s.token(0);
		target = s.token(1);
		range = atof(s.token(2).c_str());
		rof = atof(s.token(3).c_str());

	}
private:
	std::string obj;
	std::string m_info;
	std::string target;
	moving_state* ms;
	
	float range;
	bool moving;
	float rof;
	float next_attack;
};

#endif