#ifndef _CSCENE_H
#define _CSCENE_H

#include "CFrame.h"
#include "CCamera.h"
#include "..\Common\IEntity.h"


class _API CScene
{
public:
	CScene();
	~CScene();

	CFrame* Process(CFrame* thisFrame);
	VOID AddObject(IRenderable* obj);
	VOID SetFrustum(/* add parameters */);
	VOID SetCamera(CCamera* newCam);
	VOID GetCamera(CCamera* cam);
	RESULT Update();
	void SetDevice(IDevice* device);
private:
	struct __declspec(dllexport) List
	{
		List* next;
		List* prev;
		IRenderable* thisObj;
		List()
		{
			next = NULL;
			prev = NULL;
			thisObj = NULL;
		}
	};
	List* Obj;
	CCamera thisCam;
	BOOL camChanged;
	IDevice* m_device;
};

#endif