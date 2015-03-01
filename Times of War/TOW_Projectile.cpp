#include "TOW_Projectile.h"
#include "..\Common\singleton.h"
TOW_Projectile::TOW_Projectile()
{
	m_state = NULL;
	m_world = NULL;
	m_s_time = Core::GetTime();
}

TOW_Projectile::~TOW_Projectile()
{

}

RESULT TOW_Projectile::Think()
{
	m_curr_time = Core::GetTime();
	m_curr_time -= m_s_time;
	float m_int = (m_curr_time / m_time) * Core::GetGameSpeed();
	if(m_int >= 1.0f)
	{
		std::stringstream up;
		up << m_target << " damage " << m_owner << " " << properties.property(std::string("damage")).as_if_int();		
		m_world->UpdateGameObject(up.str());
		return DESTROY;
	}
	pos.x = m_start.x + m_int * (m_end.x - m_start.x);
	pos.z = m_start.z + m_int * (m_end.z - m_start.z);
	pos.y = property(std::string("height")).as_if_float();
	return OK;
}


void TOW_Projectile::SetTarget(std::string& target)
{
	m_target = target;
}

void TOW_Projectile::SetOwner(std::string& owner)
{
	m_owner = owner;
}

void TOW_Projectile::SetStart(VECTOR& s)
{
	m_start = s;
}

void TOW_Projectile::SetEnd(VECTOR& e)
{
	m_end = e;
	ray r;
	r.orig = m_start;
	r.dir = m_end - r.orig;
	
	VectorNormalize(&r.dir, &r.dir);
	r.orig += r.dir;
	TOW_GameObject* go = (TOW_GameObject*)singleton<TOW_ObjectManager*>::instance()->GetObject(m_world->HitTest2D(&r));
	if(!go)
		return;
	m_target = go->name()();
	m_end = go->pos;
}

void TOW_Projectile::SetVelocity(float v)
{
	m_v = v;
	m_time = sqrt(
			((m_start.x - m_end.x)*(m_start.x - m_end.x))+
			((m_start.z - m_end.z)*(m_start.z - m_end.z)));
	m_time /= v;
}

void TOW_Projectile::Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, std::vector<variant>& params)
{

}