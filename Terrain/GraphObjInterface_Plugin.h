#ifndef _GRAPHOBJINTERFACE_PLUGIN_H
#define _GRAPHOBJINTERFACE_PLUGIN_H

#include "..\DivGraph\IGraphPlugin.h"
#include "Terrain_Tile.h"

class _API GraphObjInterface_Plugin : public IGraphPlugin
{
public:
	GraphObjInterface_Plugin();
	~GraphObjInterface_Plugin();

	IObject* CreateObject(unsigned int id);
	CSmartPtr<IGraphObjFactory>& Factory();
	CSmartPtr<IDevice>& Device();
	void CreateTile(CSmartPtr<Terrain_Tile>& tile);

	
};

#endif