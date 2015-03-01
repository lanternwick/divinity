#ifndef _CFRAME_H
#define _CFRAME_H

#include "..\Common\result.h"
#include "..\Common\worldobj.h"
#include "IDevice.h"
#include "CCamera.h"
#include "..\Common\matrix.h"
#include "..\Common\IRenderable.h"

class _API CFrame
{
public:
	CFrame();
	~CFrame();

	RESULT Render();
	RESULT Clear(DWORD color);
	RESULT Flip();
	VOID AddObject(IRenderable* obj);
	VOID Reset();

	VOID SetDevice(CSmartPtr<IDevice> dev);
	
	VOID SetCamera(CCamera* newCam);
	VOID SetLight(LIGHT* light);
	VOID EnableLight();
private:
	CSmartPtr<IDevice> myDevice;
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
	List* Objs;
	CCamera* thisCam;
	DWORD numPrims;
};

#endif