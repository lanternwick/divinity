#include "TTerrainFactory.h"

GraphObjInterface_Plugin* TTerrainFactory::Interface = NULL;
bool TTerrainFactory::init = FALSE;

TTerrainFactory::TTerrainFactory()
{
	Interface = new GraphObjInterface_Plugin;
}

TTerrainFactory::~TTerrainFactory()
{
	delete Interface;
}

