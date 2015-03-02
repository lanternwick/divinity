#include "CCamera.h"
#include "..\Common\Matrix.h"


CCamera::CCamera()
{

}

CCamera::~CCamera()
{

}

void CCamera::set()
{
	MATRIX m;
	MatrixMatrixPerspectiveFovLH(&m, m_fov, m_aspect, m_near, m_far);
	m_device->SetTransform(T_PROJECTION, &m);
	MatrixMatrixLookAtLH(&m, &m_pos, &m_at, &m_up); 
	m_device->SetTransform(T_VIEW, &m);
}

void CCamera::set_device(IDevice* device)
{
	m_device = device;
}

void CCamera::pos(const VECTOR& v)
{
	m_pos = v;
}

void CCamera::at(const VECTOR& v)
{
	m_at = v;
}

void CCamera::up(const VECTOR& v)
{
	m_up = v;
}

void CCamera::fov(const float f)
{
	m_fov = f;
}

void CCamera::far_pl(const float f)
{
	m_far = f;
}

void CCamera::near_pl(const float n)
{
	m_near = n;
}

void CCamera::aspect(const float a)
{
	m_aspect = a;
}

const VECTOR& CCamera::pos() const
{	
	return m_pos;
}

const VECTOR& CCamera::at() const
{	
	return m_at;
}
const VECTOR& CCamera::up() const
{	
	return m_up;
}

const float CCamera::fov() const
{
	return m_fov;
}

const float CCamera::far_pl() const
{
	return m_far;	
}

const float CCamera::near_pl() const
{
	return m_near;
}

const float CCamera::aspect() const
{
	return m_aspect;
}

VECTOR* CCamera::unproject(VECTOR* r, VECTOR* src)
{
	MATRIX mProj, mView;
	
	MatrixMatrixPerspectiveFovLH(&mProj, m_fov, m_aspect, m_near, m_far);
	
	MatrixMatrixLookAtLH(&mView, &m_pos, &m_at, &m_up); 
	
	//m_device->GetTransform(T_VIEW, &mView);
	//m_device->GetTransform(T_PROJECTION, &mProj);
	VECTOR v;
	/*v.x = ( ( +( 2.0f * src->x ) / ScreenX  ) - 1 ) / mProj._11;
	v.y = ( ( -( 2.0f * src->y ) / ScreenY ) + 1 ) / mProj._22;
	v.z =  src->z;*/
	v.x = (tanf(m_aspect*0.5f)*(src->x/((float)m_device->width()*0.5f)-1.0f)/m_aspect)*src->z;
	v.y = (tanf(m_aspect*0.5f)*(1.0f-src->y/((float)m_device->height()*0.5f)))*src->z;
	v.z = src->z;
	MATRIX mViewInv;
	MatrixMatrixInverse(&mView, &mViewInv);
	//MatrixMatrixVectorApply(r, &mViewInv, &v);

	r->x  = v.x*mViewInv._11 + v.y*mViewInv._21 + v.z*mViewInv._31;
	r->y  = v.x*mViewInv._12 + v.y*mViewInv._22 + v.z*mViewInv._32;
	r->z  = v.x*mViewInv._13 + v.y*mViewInv._23 + v.z*mViewInv._33;

	VectorNormalize(&v, r);

	r->x = mViewInv._41 + src->z * v.x;
	r->y = mViewInv._42 + src->z * v.y;
	r->z = mViewInv._43 + src->z * v.z;

	return r;
}

ray* CCamera::compute_ray(ray* r, VECTOR* s)
{
	if(!r) return NULL;

	VECTOR v;
		
	MATRIX mProj, mView;
	MatrixMatrixPerspectiveFovLH(&mProj, m_fov, m_aspect, m_near, m_far);
	
	MatrixMatrixLookAtLH(&mView, &m_pos, &m_at, &m_up); 
	//m_device->GetTransform(T_VIEW, &mView);
	//m_device->GetTransform(T_PROJECTION, &mProj);

	ray r2;

	v.x = ( ( +( 2.0f * (s->x * m_device->width()) ) / m_device->width() ) - 1 ) / mProj._11;
	v.y = ( ( -( 2.0f * (s->y * m_device->height()) ) / m_device->height() ) + 1 ) / mProj._22;
	v.z =  1.0f;

	MATRIX mInvView;
	MatrixMatrixInverse(&mView, &mInvView);

	// Transform the camera space pick ray into world space
	r->dir.x  = v.x * mInvView._11 + v.y * mInvView._21 + v.z * mInvView._31;
	r->dir.y  = v.x * mInvView._12 + v.y * mInvView._22 + v.z * mInvView._32;
	r->dir.z  = v.x * mInvView._13 + v.y * mInvView._23 + v.z * mInvView._33;
	
	//MatrixMatrixVectorApply(&r->dir, &mInvView, &v);
	VectorNormalize(&r->dir, &r->dir);
	r->orig.x = mInvView._41 + m_near * r->dir.x;
	r->orig.y = mInvView._42 + m_near * r->dir.y;
	r->orig.z = mInvView._43 + m_near * r->dir.z;

	return r;
}