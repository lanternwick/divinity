#ifndef MOVING_STATE_H
#define MOVING_STATE_H

#pragma warning(disable : 4786)
#include "TOW_World.h"
#include "game_state.h"

#include <sstream>

class moving_state : public game_state
{
public:
	moving_state(){}
	moving_state(TOW_World* world){m_world = world;m_time = 0.0f;}
	virtual ~moving_state(){m_time = 0.0f;};
	virtual RESULT Think()
	{
		if(m_path.size() == 0)
		{
			return END_STATE;
		}
		float m_temp_time = Core::GetTime();
		m_temp_time -= m_s_time;
		float m_int = (m_temp_time / m_time) * Core::GetGameSpeed();
		if(m_int >= 1.0f)
		{
			if(curr_node != m_path.end())
			{
				next_node();
				m_temp_time = Core::GetTime();
				m_temp_time -= m_s_time;
				m_int = (m_temp_time / m_time) * Core::GetGameSpeed();
			}
			if(curr_node == m_path.end())
				return END_STATE;
		}
		VECTOR v;
		v.x = m_start.x + (m_int * (m_end.x - m_start.x));
		v.z = m_start.z + (m_int * (m_end.z - m_start.z));
		std::stringstream ss;
		ss << obj << " move abs " << double(v.x) << " " << double(v.z);
		m_world->UpdateGameObject(ss.str());
		return OK;
	}
	virtual void set_state_info(std::string& info)
	{
		m_info = info;
		string_tokenizer<> s(info, std::string(" "));
		obj = s.token(0);
		m_start.x = atof(s.token(1).c_str());
		m_start.z = atof(s.token(2).c_str());
		m_end.x = atof(s.token(3).c_str());
		m_end.z = atof(s.token(4).c_str());
		m_speed = atof(s.token(5).c_str());
		
		m_path = m_world->GetPathingMap()->find_path(m_start, m_end);
		if(m_path.size() == 0)
		{
			return;
		}	
		m_path.back()->position = m_end;
		curr_node = m_path.begin();
		(*curr_node)->position = m_start;
		m_end = (*(curr_node+1))->position;
		m_time = sqrt(
			((m_start.x - m_end.x)*(m_start.x - m_end.x))+
			((m_start.z - m_end.z)*(m_start.z - m_end.z)));
		m_time /= m_speed;
		m_s_time = Core::GetTime();
	}
private:
	std::string obj;
	std::string m_info;
	std::vector<TOW_PathingMap::node*> m_path;
	std::vector<TOW_PathingMap::node*>::iterator curr_node;
	VECTOR m_start;
	VECTOR m_end;
	
	float m_speed;
	float m_time;
	float m_s_time;
	void next_node()
	{
		curr_node++;
		if(curr_node+1 == m_path.end())
		{
			curr_node+=1;
			return;
		}
		m_start = (*curr_node)->position;
		
		m_end = (*(curr_node+1))->position;
		m_time = sqrt(
			((m_start.x - m_end.x)*(m_start.x - m_end.x))+
			((m_start.z - m_end.z)*(m_start.z - m_end.z)));
		m_time /= m_speed;
		m_s_time = Core::GetTime();
	}
};

#endif