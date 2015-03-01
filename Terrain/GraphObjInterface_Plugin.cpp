#include "GraphObjInterface_Plugin.h"

GraphObjInterface_Plugin::GraphObjInterface_Plugin()
{
	pluginName = new char[64];
	strcpy(pluginName, "GraphObjInterface_Mod");
}

GraphObjInterface_Plugin::~GraphObjInterface_Plugin()
{
	myDevice.Release();
	myFactory.Release();
	
}

IObject* GraphObjInterface_Plugin::CreateObject(unsigned int id)
{
	
	return NULL;	
}

CSmartPtr<IGraphObjFactory>& GraphObjInterface_Plugin::Factory()
{
	return myFactory;
}

CSmartPtr<IDevice>& GraphObjInterface_Plugin::Device()
{
	return myDevice;
}

void GraphObjInterface_Plugin::CreateTile(CSmartPtr<Terrain_Tile>& tile)
{
	//if(!init) throw "Terrain Plugin not Initialized";
	tile.Attach(new Terrain_Tile(1));
	tile->SetFactory(Factory());
	tile->SetDevice(Device());
}