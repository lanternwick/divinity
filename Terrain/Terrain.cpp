#include "stdafx.h"

#include "Terrain.h"
//#include "TTerrainFactory.h"
#include "GraphObjInterface_Plugin.h"



//TTerrainFactory* factory = NULL; 
IGraphPlugin* instance;

BOOL __stdcall DllMain(HANDLE hModule, 
					   DWORD ul_reason_for_call, 
					   LPVOID lpReserved)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	
		break;
	case DLL_PROCESS_DETACH:
		
		break;
	}
	return true; 
}




IGraphPlugin* GetGraphPlugin()
{
	instance = new GraphObjInterface_Plugin;

	return instance;
}

__declspec(dllexport) void CreateTile(CSmartPtr<Terrain_Tile>& tile)
{
	tile.Release();
	tile.Attach((Terrain_Tile*)instance->CreateObject(0));
}

