
#include "CScene.h"

CScene::CScene()
{
	Obj = NULL;
	camChanged = FALSE;
}

CScene::~CScene()
{
	List* c = Obj;
	while(c)
	{
		Obj = c;
		c = c->next;
		delete Obj->thisObj;
		delete Obj;
	}
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
	CScene::List* c = Obj;
	thisFrame->Reset();
	while(c)
	{
		thisFrame->AddObject(c->thisObj);
		c = c->next;
	}
	if(camChanged)
	{
		thisFrame->SetCamera(thisCam);
		camChanged = FALSE;
	}
	return thisFrame;
}

VOID CScene::SetFrustrum()
{
	return;
}

VOID CScene::SetCamera(CCamera* newCam)
{	
	// set the camera
	thisCam = newCam;
	camChanged = TRUE;
	return;
}

RESULT CScene::Update()
{
/*	// update objects
	RESULT r = OK;
	IRenderable* c = Obj;
	while(c)
	{
		r = c->Update();
		if(RFAILED(r))
			return r;
		c = (WorldObj*)c->Sibling;
	}
	return r;	*/
	return OK;
}