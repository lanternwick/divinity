#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "IDevice.h"
#include "..\Common\result.h"
#include "CFrame.h"
#include "CScene.h"
#include "IGraphObjFactory.h"
#include "StaticMesh.h"
#include "IGraphPlugin.h"

struct DivGraphInitSettings
{
	HWND hWnd;
	UINT cx;
	UINT cy;
	BOOL Windowed;
};

enum DIV_DRAWMODES
{
	D_DM_SOLID,
	D_DM_WIREFRAME,
};

enum DIV_CULLMODES
{
	D_CM_CW,
	D_CM_CCW,
	D_CM_NONE,
};


class _API Graphics
{
public:
	~Graphics();

	static Graphics* Instance();
	
	RESULT Initialize(DivGraphInitSettings& iSet);
	CScene* CreateScene();
	CFrame* CreateFrame();
	RESULT LoadStaticMesh(char* filename, char* id, StaticMesh*& mesh);
	RESULT LoadPlugin(const char* plugin, IGraphPlugin*& p_plugin_ret);
	RESULT SetTransform(TRANSFORMATION t, MATRIX* m);
	RESULT SetDrawMode(DIV_DRAWMODES dm);
	RESULT SetCullMode(DIV_CULLMODES cm);
	RESULT EnableBlending(bool setBlend);
	RESULT SetScene(CScene* scene);
	RESULT GetScene(CScene* scene);
	VECTOR* Unproject(VECTOR* r, VECTOR* src);

private:
	Graphics();
	static Graphics* _instance;
	IDevice* myDevice;
	IGraphObjFactory* factory;
	IGraphPlugin* plugins;
	CScene* currScene;
};

#endif