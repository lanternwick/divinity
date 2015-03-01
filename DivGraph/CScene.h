#ifndef _CSCENE_H
#define _CSCENE_H

#include "CFrame.h"
#include "CCamera.h"
#include "..\Common\IRenderable.h"


class _API CScene
{
public:
	CScene();
	~CScene();

	CFrame* Process(CFrame* thisFrame);
	VOID AddObject(IRenderable* obj);
	VOID SetFrustrum(/* add parameters */);
	VOID SetCamera(CCamera* newCam);
	RESULT Update();
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
	CCamera* thisCam;
	BOOL camChanged;
};

#endif