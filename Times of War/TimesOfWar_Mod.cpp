#include "stdafx.h"
#include "TimesOfWar_Mod.h"

class Mesh : public IRenderable
{
public:
	CSmartPtr<StaticMesh> mesh;
	~Mesh() { mesh.Release(); }
	
	virtual RESULT Render(){mesh->Render(); return OK;}
	
};

class Tile : public IRenderable
{
public:
	CSmartPtr<Terrain_Tile> mesh;
	~Tile() { mesh.Release(); }
	
	virtual RESULT Render(){mesh->Render(); return OK;}
	
};


static VOID DoEvents(HWND hWnd)
{
	MSG msg;
	if(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}

TimesOfWar_Mod::TimesOfWar_Mod()
{
	Core::Function_Enter("TimesOfWar::Constructor()");
	engine = new CEngine;
	Core::Function_Exit();
}

TimesOfWar_Mod::~TimesOfWar_Mod()
{
	Core::Function_Enter("TimesOfWar::Destructor()");
	Core::Function_Exit();

}

RESULT TimesOfWar_Mod::DrawFrame(CFrame* frameToRender)
{
	Core::Function_Enter("TimesOfWar::DrawFrame()");

	frameToRender->Clear(0xff0000ff);
	frameToRender->Render();
	frameToRender->Flip();

	Core::Function_Exit();
	return OK;
}

RESULT TimesOfWar_Mod::UpdateFrame(CScene* sceneToUpdate)
{
	Core::Function_Enter("TimesOfWar::UpdateFrame()");
	
	Core::Function_Exit();
	return OK;
}

INT TimesOfWar_Mod::Run(char* cmdLine)
{
	
	Core::Function_Enter("TimesOfWar::Run()");
	DWORD numPrims = 0;
	GraphObjInterface_Plugin* terrainPlugin = NULL;

	DivGraphInitSettings gSettings = { hWnd, 640, 480, TRUE };
	
	// intialize graphics subsystem
	engine->EGraphics()->Initialize(gSettings);
	
	

	// fill
	engine->EGraphics()->SetDrawMode(D_DM_SOLID);
	engine->EGraphics()->SetCullMode(D_CM_NONE);

	

	CScene* currScene = NULL;
	CFrame* currFrame = NULL;

	currScene = engine->EGraphics()->CreateScene();
	currFrame = engine->EGraphics()->CreateFrame();

	Mesh* mesh = new Mesh;
	engine->EGraphics()->LoadStaticMesh("C:\\chest.model", "Tower", mesh->mesh);
	mesh->mesh->SetDimensions(&VECTOR(1.0f, 1.5f, 0.6f));
	currScene->AddObject(mesh);

	engine->EGraphics()->LoadPlugin("terrain.dll", (IGraphPlugin*&)terrainPlugin);
	Tile* tile = new Tile;
	Tile* tile2 = new Tile;
	Tile* tile3 = new Tile;
	Tile* tile4 = new Tile;
	terrainPlugin->CreateTile(tile->mesh);
	terrainPlugin->CreateTile(tile2->mesh);
	terrainPlugin->CreateTile(tile3->mesh);
	terrainPlugin->CreateTile(tile4->mesh);
	tile->mesh->Create(&VECTOR(0.0f, 0.0f, 0.0f), &VECTOR(0.0f, 0.0f, 0.0f), 6.0f, 25);
	tile2->mesh->Create(&VECTOR(-6.0f, 0.0f, 0.0f), &VECTOR(0.0f, 0.0f, 0.0f), 6.0f, 25);
	tile3->mesh->Create(&VECTOR(-6.0f, 0.0f, -6.0f), &VECTOR(0.0f, 0.0f, 0.0f), 6.0f, 25);
	tile4->mesh->Create(&VECTOR(0.0f, 0.0f, -6.0f), &VECTOR(1.0f, 0.0f, 0.0f), 6.0f, 25);
	tile->mesh->DrawSimple();
	tile2->mesh->DrawSimple();
	tile3->mesh->DrawSimple();
	tile4->mesh->DrawSimple();
	currScene->AddObject(tile);
	currScene->AddObject(tile2);
	currScene->AddObject(tile3);
	currScene->AddObject(tile4);

	CCamera thisCam;
	// define camera
	thisCam.fAspect = 1.0f;
	thisCam.fFov = 3.14159f / 2.0f;
	thisCam.fFar = 1000.0f;
	thisCam.fNear = 0.1f;
	thisCam.vecAt = VECTOR(0.0f, 0.0f, 0.0f);
	thisCam.vecPos = VECTOR(5.0f, 5.0f, -5.0f);
	thisCam.vecUp = VECTOR(0.0f, 1.0f, 0.0f);
	
	// set camera
	currScene->SetCamera(&thisCam);
	
	//set up a light
	LIGHT l;
	ZeroMemory(&l, sizeof(l));
	
	l.Ambient.r = 1.0f;
	l.Ambient.g = 1.0f;
	l.Ambient.b = 1.0f;

	
	l.Diffuse.r = 0.6f;
	l.Diffuse.g = 0.6f;
	l.Diffuse.b = 0.6f;

	l.Type = LIGHT_POINT;
	l.Range = 7.0f;
	l.Position = VECTOR(5.0f, 5.0f, 5.0f);
	l.Attenuation0 = 0.5f;
	currFrame->SetLight(&l);
	currFrame->EnableLight();

	engine->bRunning(true);
	while(engine->bRunning())
	{
		engine->ECore()->BeginFrame();
		DoEvents(hWnd);
		UpdateFrame(currScene);
		currFrame = currScene->Process(currFrame);
		DrawFrame(currFrame);
		engine->ECore()->EndFrame();
	}
	delete terrainPlugin;
	delete currFrame;
	delete currScene;
	
	Core::Function_Exit();
	return (int)(engine->ECore()->GetTimeDelta()*1000);
}

RESULT TimesOfWar_Mod::PreCache(char* file)
{
	return OK;
}