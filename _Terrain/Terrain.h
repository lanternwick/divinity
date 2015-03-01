
#ifdef _EXPORTS
#define _API __declspec(dllexport)
#else
#define _API __declspec(dllimport)
#endif

#include "Terrain_Tile.h"
#include "GraphObjInterface_Plugin.h"

_API void CreateTile(Terrain_Tile*& tile);