#include "GraphObjInterface_Plugin.h"

GraphObjInterface_Plugin::GraphObjInterface_Plugin()
{
	Core::Function_Enter("GraphObjInterface_Plugin::Constructor()");
	pluginName = new char[64];
	strcpy(pluginName, "GraphObjInterface_Mod");
	Core::Function_Exit();
}

GraphObjInterface_Plugin::~GraphObjInterface_Plugin()
{
	Core::Function_Enter("GraphObjInterface_Plugin::Destructor()");
	Core::Function_Exit();
}

IObject* GraphObjInterface_Plugin::CreateObject(unsigned int id)
{
	Core::Function_Enter("GraphObjInterface_Plugin::CreateObject()");
	Core::Function_Exit();
	return NULL;	
}

IGraphObjFactory* GraphObjInterface_Plugin::Factory()
{
	Core::Function_Enter("GraphObjInterface_Plugin::Factory()");
	IGraphObjFactory* p = myFactory;
	Core::Function_Exit();
	return myFactory;
}

IDevice* GraphObjInterface_Plugin::Device()
{
	Core::Function_Enter("GraphObjInterface_Plugin::Device()");
	IDevice* p = myDevice;
	Core::Function_Exit();
	return myDevice;
}

void GraphObjInterface_Plugin::CreateTile(Terrain_Tile*& tile)
{
	//if(!init) throw "Terrain Plugin not Initialized";
	Core::Function_Enter("GraphObjInterface_Plugin::CreateTile()");
	tile = new Terrain_Tile;
	tile->SetFactory(Factory());
	tile->SetDevice(Device());
	Core::Function_Exit();
	return;
}