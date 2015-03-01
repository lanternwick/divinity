#ifndef _CCAMERA_H
#define _CCAMERA_H

class _API CCamera
{
public:
	VECTOR vecPos;
	VECTOR vecAt;
	VECTOR vecUp;
	float fFov;
	float fNear;
	float fFar;
	float fAspect;
};

#endif