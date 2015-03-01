#ifndef TOW_PROJECTILE_H
#define TOW_PROJECTILE_H

#include "TOW_GameObject.h"
#include "TOW_World.h"

class TOW_Projectile : public TOW_GameObject
{
public:
	TOW_Projectile();
	~TOW_Projectile();

	RESULT Think();
	

	void SetStart(VECTOR& s);
	void SetEnd(VECTOR& e);

	void SetVelocity(float v);
	void SetTarget(std::string& target);
	void SetOwner(std::string& owner);

	virtual void Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, std::vector<variant>& params);

private:
	VECTOR m_start;
	VECTOR m_end;
	float m_v;
	float m_s_time;
	float m_time;
	float m_curr_time;
	std::string m_owner;
	std::string m_target;
};

#endif
