
#include "stdafx.h"

#include "Graphics.h"
#include "CFrame.h"
#include "CScene.h"

Graphics* Graphics::_instance = NULL;

Graphics::Graphics()
{
	// create new rendering device
	Core::Function_Enter("Graphics::Constructor()");
	myDevice = new D3DDevice;
	Core::Function_Exit();
	
}

Graphics::~Graphics()
{
	Core::Function_Enter("Graphics::Destructor");
	myDevice->Cleanup();
	delete myDevice;
	delete factory;
	Core::Function_Exit();
	
	

}

Graphics* Graphics::Instance()
{
	Core::Function_Enter("Graphics::Instance()");
	if(_instance == NULL)
		_instance = new Graphics;
	Core::Function_Exit();
	return _instance;
}

RESULT Graphics::Initialize(DivGraphInitSettings& iSet)
{
	Core::Function_Enter("Graphics::Initialize()");
	// create new instance
	Graphics::Instance();
	if(!myDevice)
	{
		Core::Function_Exit();
		return OUTOFMEMORY; // ut oh
	}
	// create the rendering device
	RESULT r = myDevice->Create(iSet.hWnd, iSet.Windowed, iSet.cx, iSet.cy);
	if(RFAILED(r))
	{
		Core::Function_Exit();
		return r;
	}
	// create a device object factory
	factory = new D3DGraphObjFactory;
	// and set the device
	factory->SetDevice(myDevice);
	Core::Function_Exit();
	return OK;
}

CScene* Graphics::CreateScene()
{
	// create new scene
	Core::Function_Enter("Graphics::CreateScene()");
	Core::Function_Exit();
	return new CScene;
}

CFrame* Graphics::CreateFrame()
{
	// create new frame
	Core::Function_Enter("Graphics::CreateFrame()");
	CFrame* frm = NULL;
	frm = new CFrame;
	if(frm == NULL)
	{
		Core::Function_Exit();
		return NULL; // out of memory
	}
	frm->SetDevice(myDevice); // set the device
	Core::Function_Exit();
	return frm; // return it
}

RESULT Graphics::LoadStaticMesh(char* filename, char* id, StaticMesh*& mesh)
{
	// load a mesh
	Core::Function_Enter("Graphics::LoadStaticMesh()");
	mesh = new StaticMesh(id); // new mesh
	if(!mesh)
	{
		Core::Function_Exit();
		return OUTOFMEMORY; // out of memory
	}
	RESULT r = mesh->LoadMesh(factory, filename); // load the mesh
	if(RFAILED(r))
	{
		Core::Function_Exit();
		return r;
	}
	mesh->SetDevice(myDevice); // set the device
	Core::Function_Exit();
	return OK;

}

RESULT Graphics::SetTransform(TRANSFORMATION t, MATRIX* m)
{
	Core::Function_Enter("Graphics::SetTransform()");
	RESULT r = myDevice->SetTransform(t, m); // set transformation matrix
	Core::Function_Exit();
	return r;
}

RESULT Graphics::SetDrawMode(DIV_DRAWMODES dm)
{
	// set solid or wireframe rendering
	Core::Function_Enter("Graphics::SetDrawMode()");
	RESULT r;
	switch(dm)
	{
	case D_DM_SOLID:
		r =  myDevice->SetRenderState(RS_FILLMODE, FILL_SOLID);
		break;
	case D_DM_WIREFRAME:
		r =  myDevice->SetRenderState(RS_FILLMODE, FILL_WIREFRAME);
		break;
	default:
		r = INVALIDDATA;
	}
	Core::Function_Exit();
	return r;
}

RESULT Graphics::SetCullMode(DIV_CULLMODES cm)
{
	// set Clockwise, Counter-Clockwise, or no Culling
	Core::Function_Enter("Graphics::SetCullMode()");
	RESULT r;
	switch(cm)
	{
	case D_CM_CW:
		r = myDevice->SetRenderState(RS_CULLMODE, CULL_CW);
		break;
	case D_CM_CCW:
		r = myDevice->SetRenderState(RS_CULLMODE, CULL_CCW);
		break;
	case D_CM_NONE:
		r = myDevice->SetRenderState(RS_CULLMODE, CULL_NONE);
		break;
	default:
		r = INVALIDDATA;
	}
	Core::Function_Exit();
	return r;
}

RESULT Graphics::EnableBlending(bool setBlend)
{
	Core::Function_Enter("Graphics::EnableBlending()");
	RESULT r = myDevice->SetRenderState(RS_ALPHABLENDENABLE, setBlend);
	Core::Function_Exit();
	return r;
}
		

RESULT Graphics::LoadPlugin(const char* plugin, IGraphPlugin*& p_plugin_ret)
{
	// load a graphics plugin
	Core::Function_Enter("Graphics::LoadPlugin()");
	//define pointer retrieval function
	typedef IGraphPlugin* GetGraphPlugin();
	GetGraphPlugin* graphPluginLoad = NULL;
	HMODULE mod;
	// load the DLL
	mod = LoadLibrary(plugin);
	if(!mod) // if it doesnt exist...
	{
		Core::Function_Exit();
		return FAILED_OPEN; // we failed
	}
	// get the function
	graphPluginLoad = (GetGraphPlugin*)GetProcAddress(mod, "GetGraphPlugin"); 
	if(!graphPluginLoad) // if we couldnt get the function
	{
		Core::Function_Exit();
		return INVALIDDATA; // couldnt load the plugin
	}
	IGraphPlugin* p = graphPluginLoad();
	p_plugin_ret = p; // else, get the plugin

	// add it to the list
	p_plugin_ret->nextPlugin = plugins;
	plugins = p; 

	// set the device and factory
	p_plugin_ret->SetFactory(factory);
	p_plugin_ret->SetDevice(myDevice);
	
	Core::Function_Exit();
	return OK;
}

RESULT Graphics::SetScene(CScene* scene)
{
	if(!scene)
		return INVALIDDATA;
	currScene = scene;
	return OK;
}
	
RESULT Graphics::GetScene(CScene* scene)
{
	scene = currScene;
	return OK;
}

VECTOR* Graphics::Unproject(VECTOR* r, VECTOR* src)
{
	CCamera c;
	currScene->GetCamera(&c);
	myDevice->Unproject(r, src, c.fAspect);
	return r;
}