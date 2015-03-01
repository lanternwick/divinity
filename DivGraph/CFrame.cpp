#include "CFrame.h"
#include "..\Common\result.h"

CFrame::CFrame()
{
	Objs = NULL;
}

CFrame::~CFrame()
{

}

RESULT CFrame::Render()
//renders all objects in a frame
{
	Core::Function_Enter("CFrame::Render()");
	List* c = Objs;
	// set up perspective and view matrices
	MATRIX mat;
	MatrixMatrixPerspectiveFovLH(&mat, thisCam->fFov, thisCam->fAspect, thisCam->fNear, thisCam->fFar);
	myDevice->SetTransform(T_PROJECTION, &mat);
	MatrixMatrixLookAtLH(&mat, &thisCam->vecPos, &thisCam->vecAt, &thisCam->vecUp); 
	myDevice->SetTransform(T_VIEW, &mat);

	MATRIX* mat2 = new MATRIX;
	MatrixMatrixIdentity(&mat2);
	// begin frame
	myDevice->BeginScene();
	while(c)
	{
		// set id matrix
		myDevice->SetTransform(T_WORLD, mat2);
		// render
		c->thisObj->Render();
		// next object
		c = c->next;
	}
	delete mat2;
	// end frame
	myDevice->EndScene();
	Core::Function_Exit();
	return OK;
}

RESULT CFrame::Clear(DWORD color)
{
	// clear back buffer, and start time delta counter
	Core::Function_Enter("CFrame::Clear()");
	Core::Instance()->BeginFrame();
	myDevice->Clear(color);
	Core::Function_Exit();
	return OK;
}

RESULT CFrame::Flip()
{
	// draw to screen, and stop time delta counter
	Core::Function_Enter("CFrame::Flip()");
	myDevice->Flip();
	Core::Instance()->EndFrame();
	Core::Function_Exit();
	return OK;
}

VOID CFrame::AddObject(IRenderable* obj)
{
	// add an object to the scene
	Core::Function_Enter("CFrame::AddObject()");
	List* nList = NULL;
	if(Objs == NULL)
	{
		Objs = new List;
		Objs->thisObj = obj;
	}
	else
	{
		nList = new List;
		if(nList == NULL)
			return;
		nList->thisObj = obj;
		nList->next = Objs;
		Objs->prev = nList;
		Objs = nList;
	}
	Core::Function_Exit();
	return;
}

VOID CFrame::SetDevice(CSmartPtr<IDevice> dev)
{
	// set the rendering device
	Core::Function_Enter("CFrame::SetDevice");
	myDevice = dev;
	Core::Function_Exit();
}

VOID CFrame::Reset()
// clears everything from the frame
{
	Core::Function_Enter("CFrame::Reset()");
	List* c = Objs;
	List* temp = Objs;

	while(c)
	{
		temp = c;
		c = c->next;
		delete temp;
	}
	Objs = NULL;
	Core::Function_Exit();
	return;
}

VOID CFrame::SetCamera(CCamera* newCam)
{
	// set the camera
	Core::Function_Enter("CFrame::SetCamera()");
	thisCam = newCam;
	Core::Function_Exit();
	return;
}


VOID CFrame::SetLight(LIGHT* light)
{
	// set one light
	Core::Function_Enter("CFrame::SetLight()");
	myDevice->SetLight(0, light);
	Core::Function_Exit();
}

VOID CFrame::EnableLight()
{
	// enable the light
	Core::Function_Enter("CFrame::EnableLight()");
	myDevice->LightEnable(0, TRUE);
	Core::Function_Exit();
}