
#include "stdafx.h"

#include "Graphics.h"
#include "CFrame.h"
#include "CScene.h"

Graphics* Graphics::_instance = NULL;

Graphics::Graphics()
{
	// create new rendering device
	myDevice.Attach(new D3DDevice);
	
}

Graphics::~Graphics()
{
	myDevice.Release();
	factory.Release();
	
	

}

Graphics* Graphics::Instance()
{
	if(_instance == NULL)
		_instance = new Graphics;
	return _instance;
}

RESULT Graphics::Initialize(DivGraphInitSettings& iSet)
{
	// create new instance
	Graphics::Instance();
	if(myDevice.Null())
		return OUTOFMEMORY; // ut oh
	// create the rendering device
	RESULT r = myDevice->Create(iSet.hWnd, iSet.Windowed, iSet.cx, iSet.cy);
	if(RFAILED(r))
		return r;
	// create a device object factory
	factory.Attach(new D3DGraphObjFactory);
	// and set the device
	factory->SetDevice(myDevice);

	return OK;
}

CScene* Graphics::CreateScene()
{
	// create new scene
	return new CScene;
}

CFrame* Graphics::CreateFrame()
{
	// create new frame
	CFrame* frm = NULL;
	frm = new CFrame;
	if(frm == NULL)
		return NULL; // out of memory
	frm->SetDevice(myDevice); // set the device
	return frm; // return it
}

RESULT Graphics::LoadStaticMesh(char* filename, char* id, CSmartPtr<StaticMesh>& mesh)
{
	// load a mesh
	mesh.Release();
	mesh.Attach(new StaticMesh); // new mesh
	if(mesh.Null())
		return OUTOFMEMORY; // out of memory
	RESULT r = mesh->LoadMesh(factory, filename); // load the mesh
	if(RFAILED(r))
		return r;
	mesh->SetDevice(myDevice); // set the device
	return OK;

}

RESULT Graphics::SetTransform(TRANSFORMATION t, MATRIX* m)
{
	return myDevice->SetTransform(t, m); // set transformation matrix
}

RESULT Graphics::SetDrawMode(DIV_DRAWMODES dm)
{
	// set solid or wireframe rendering
	switch(dm)
	{
	case D_DM_SOLID:
		return myDevice->SetRenderState(RS_FILLMODE, FILL_SOLID);
	case D_DM_WIREFRAME:
		return myDevice->SetRenderState(RS_FILLMODE, FILL_WIREFRAME);
	default:
		return INVALIDDATA;
	}
}

RESULT Graphics::SetCullMode(DIV_CULLMODES cm)
{
	// set Clockwise, Counter-Clockwise, or no Culling
	switch(cm)
	{
	case D_CM_CW:
		return myDevice->SetRenderState(RS_CULLMODE, CULL_CW);
	case D_CM_CCW:
		return myDevice->SetRenderState(RS_CULLMODE, CULL_CCW);
	case D_CM_NONE:
		return myDevice->SetRenderState(RS_CULLMODE, CULL_NONE);
	default:
		return INVALIDDATA;
	}
}

RESULT Graphics::LoadPlugin(const char* plugin, IGraphPlugin*& p_plugin_ret)
{
	// load a graphics plugin

	//define pointer retrieval function
	typedef IGraphPlugin* GetGraphPlugin();
	GetGraphPlugin* graphPluginLoad = NULL;
	HMODULE mod;
	// load the DLL
	mod = LoadLibrary(plugin);
	if(!mod) // if it doesnt exist...
		return FAILED_OPEN; // we failed
	// get the function
	graphPluginLoad = (GetGraphPlugin*)GetProcAddress(mod, "GetGraphPlugin"); 
	if(!graphPluginLoad) // if we couldnt get the function
		return INVALIDDATA; // couldnt load the plugin
	IGraphPlugin* p = graphPluginLoad();
	p_plugin_ret = p; // else, get the plugin

	// add it to the list
	p_plugin_ret->nextPlugin = plugins;
	plugins = p; 

	// set the device and factory
	p_plugin_ret->SetFactory(factory);
	p_plugin_ret->SetDevice(myDevice);

	return OK;
}