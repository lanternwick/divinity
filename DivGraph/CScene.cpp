
#include "CScene.h"

CScene::CScene()
{
	Core::Function_Enter("CScene::Constructor()");
	Obj = NULL;
	camChanged = FALSE;
	Core::Function_Exit();
}

CScene::~CScene()
{
	Core::Function_Enter("CScene::Destructor()");
	
	Core::Function_Exit();
}

VOID CScene::AddObject(IRenderable* obj)
{
	Core::Function_Enter("CScene::AddObject()");
	CScene::List* nList = NULL;
	if(Obj == NULL)
	{
		Obj = new List;
		Obj->thisObj = obj;
	}
	else
	{
		nList = new List;
		if(nList == NULL)
			return;
		nList->thisObj = obj;
		nList->next = Obj;
		Obj->prev = nList;
		Obj = nList;
	}
	Core::Function_Exit();
	return;
}

CFrame* CScene::Process(CFrame* thisFrame)
{
	// process this scene into a frame
	Core::Function_Enter("CScene::Process()");
	CScene::List* c = Obj;
	thisFrame->Reset();
	while(c)
	{
		thisFrame->AddObject(c->thisObj);
		c = c->next;
	}
	thisFrame->SetCamera(&thisCam);
	/*if(camChanged)
	{
		
		camChanged = FALSE;
	}*/
	Core::Function_Exit();
	return thisFrame;
}

VOID CScene::SetFrustum()
{
	Core::Function_Enter("CScene::SetFrustum()");

	Core::Function_Exit();
	return;
}

VOID CScene::SetCamera(CCamera* newCam)
{	
	// set the camera
	Core::Function_Enter("CScene::SetCamera()");
	thisCam = *newCam;
	camChanged = TRUE;
	MATRIX mat;
	//MatrixMatrixOrthographicLH(&mat, myDevice->width(), myDevice->height(), thisCam->fNear, thisCam->fFar);
	MatrixMatrixPerspectiveFovLH(&mat, thisCam.fFov, thisCam.fAspect, thisCam.fNear, thisCam.fFar);
	m_device->SetTransform(T_PROJECTION, &mat);
	MatrixMatrixLookAtLH(&mat, &thisCam.vecPos, &thisCam.vecAt, &thisCam.vecUp); 
	m_device->SetTransform(T_VIEW, &mat);
	Core::Function_Exit();
	return;
}

VOID CScene::GetCamera(CCamera* cam)
{
	Core::Function_Enter("CScene::SetCamera()");
	*cam = thisCam;
	Core::Function_Exit();
}

RESULT CScene::Update()
{
	Core::Function_Enter("CScene::Update()");
	Core::Function_Exit();
	return OK;
}

void CScene::SetDevice(IDevice* device)
{
	m_device = device;
}