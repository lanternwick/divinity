#ifndef TOW_GAMEBUIDLING_H
#define TOW_GAMEBUILDING_H

#include "TOW_GameObject.h"

class TOW_GameBuilding : public TOW_GameObject
{
public:
	TOW_GameBuilding(char* id);
	virtual ~TOW_GameBuilding();

	virtual RESULT Think();
	virtual void Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, custom::vector<variant>& params);
private:
	TOW_GameBuilding();
};

#endif