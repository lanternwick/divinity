#ifndef GAME_STATE_H
#define GAME_STATE_H

//#include "TOW_World.h"

class TOW_World;

class game_state
{
public:
	game_state(){}
	game_state(TOW_World* world){m_world = world;}
	virtual ~game_state(){}
	virtual RESULT Think()=0;
	virtual void set_state_info(std::string& info_string){};
	virtual void StateExit(){}
	virtual void StateEnter(){}
protected:
	TOW_World* m_world;
};

#endif