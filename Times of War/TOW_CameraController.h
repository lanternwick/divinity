#ifndef TOW_CAMERACONTROLLER_H
#define TOW_CAMERACONTROLLER_H

#include "..\DivGraph\CCamera.h"
#include "..\Common\result.h"
#include "..\Common\rect_temp.h"
class TOW_CameraController
{
public:
	TOW_CameraController();
	~TOW_CameraController();

	void Attach(CCamera* camera);
	
	RESULT Think();
	RESULT Render();

	void Dir(VECTOR& v);
	const VECTOR& Dir();
	void Velocity(float v);
	const float Velocity();
	
	void LimitCamera(bool limit);
	const bool LimitCamera();
	void LimitRect(rect<float>& limit_rect);
	const rect<float>& LimitRect();
	void LimitPlane(PLANE& lp);
	const PLANE& LimitPlane();

protected:
	CCamera* m_camera;
	VECTOR m_dir;
	float m_vel;
	bool m_limit;
	rect<float> m_limit_rect;
	PLANE m_camera_intersect_plane_limit;

};

#endif