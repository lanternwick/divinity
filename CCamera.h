#ifndef _CCAMERA_H
#define _CCAMERA_H

#include "IDevice.h"
#include "VECTOR.h"
#include "ray.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	void set_device(IDevice* device);
	void set();
	void pos(const VECTOR& v);
	void at(const VECTOR& v);
	void up(const VECTOR& v);
	void fov(const float f);
	void far_pl(const float f);
	void near_pl(const float n);
	void aspect(const float a);
	const VECTOR& pos() const;
	const VECTOR& at() const;
	const VECTOR& up() const;
	const float fov() const;
	const float far_pl() const;
	const float near_pl() const;
	const float aspect() const;
	
	VECTOR* unproject(VECTOR* r, VECTOR* src);
	ray* compute_ray(ray* r, VECTOR* s);

private:
	IDevice* m_device;
	VECTOR m_pos;
	VECTOR m_at;
	VECTOR m_up;
	VECTOR m_dir;
	float m_fov;
	float m_near;
	float m_far;
	float m_aspect;
	float m_velocity;
};

#endif