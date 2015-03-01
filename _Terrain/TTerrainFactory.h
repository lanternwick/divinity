#ifndef _TTERRAINFACTORY_H
#define _TTERRAINFACTORY_H

#include "Terrain_Tile.h"

class TTerrainFactory
{
public:
	TTerrainFactory();
	~TTerrainFactory();

	void CreateTile(CSmartPtr<Terrain_Tile>& tile);

	static GraphObjInterface_Plugin* Interface;
	static bool init;
};

#endif
