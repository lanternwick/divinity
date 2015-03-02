#include "TOW_CameraController.h"

TOW_CameraController::TOW_CameraController()
{
	m_camera = NULL;
	m_limit = false;
}

TOW_CameraController::~TOW_CameraController()
{
	m_camera = NULL;
}

void TOW_CameraController::Attach(CCamera* camera)
{
	m_camera = camera;
}
	
RESULT TOW_CameraController::Think()
{
	if(!m_camera)
	{
		return OK;
	}

	ray rays[3];
	VECTOR gtemp[3];
	rect<float> g_box;
	

	VECTOR ps(m_camera->pos());
	VECTOR as(m_camera->at());
	VECTOR po(ps);
	VECTOR ao(as);

	po += m_dir * m_vel * Core::GetRawTimeDelta();
	
	
	ao += m_dir * m_vel * Core::GetRawTimeDelta();
	m_camera->pos(po);
	m_camera->at(ao);
	if(!m_limit)
	{
		return OK;
	}
	m_camera->compute_ray(&rays[0], &VECTOR(0, 0, 0.1f));
	m_camera->compute_ray(&rays[1], &VECTOR(1, 0, 0.1f));
	m_camera->compute_ray(&rays[2], &VECTOR(1, 1, 0.1f));

	rays[0].intersect_plane(&gtemp[0], &m_camera_intersect_plane_limit);
	rays[1].intersect_plane(&gtemp[1], &m_camera_intersect_plane_limit);
	rays[2].intersect_plane(&gtemp[2], &m_camera_intersect_plane_limit);

	g_box.left = gtemp[0].x;
	g_box.right = gtemp[1].x;
	g_box.top = gtemp[0].z;
	g_box.bottom = gtemp[2].z;
	if((g_box.left < m_limit_rect.left) || (g_box.right > m_limit_rect.right))
	{
		ao.x = as.x;
		po.x = ps.x;
	}
	if((g_box.top > m_limit_rect.top) || (g_box.bottom < m_limit_rect.bottom))
	{		
		ao.z = as.z;
		po.z = ps.z;
	}
	m_camera->pos(po);
	m_camera->at(ao);
	
	return OK;
}

RESULT TOW_CameraController::Render()
{
	if(m_camera)
	{
		m_camera->set();
	}
	return OK;
}

void TOW_CameraController::Dir(VECTOR& v)
{
	m_dir = v;
}

const VECTOR& TOW_CameraController:: Dir()
{
	return m_dir;
}

void TOW_CameraController::Velocity(float v)
{
	m_vel = v;
}

const float TOW_CameraController::Velocity()
{
	return m_vel;
}

void TOW_CameraController::LimitCamera(bool limit)
{
	m_limit = limit;
}

const bool TOW_CameraController::LimitCamera()
{
	return m_limit;
}

void TOW_CameraController::LimitRect(rect<float>& limit_rect)
{
	m_limit_rect = limit_rect;
}

const rect<float>& TOW_CameraController::LimitRect()
{
	return m_limit_rect;
}

void TOW_CameraController::LimitPlane(PLANE& lp)
{
	m_camera_intersect_plane_limit = lp;
}

const PLANE& TOW_CameraController::LimitPlane()
{
	return m_camera_intersect_plane_limit;
}
