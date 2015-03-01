#ifndef _TOW_MAP_H
#define _TOW_MAP_H

#include "stdafx.h"
#include "..\DivCore\DivCore.h"
#include "..\DivGraph\DivGraph.h"
#include "IHeightMap.h"
#include "TOW_HeightMap.h"
#include "TOW_TextureMap.h"
#include "Terrain_Tile.h"
#include "..\Common\IEntity.h"
#include <vector>
#include  "..\DivNet\divsock\mutex.h"
#define FCF_CLIENT

extern "C"{
	#include <compf>
}

#include "TOW_ObjectManager.h"

class TOW_Map : public TOW_ObjectManager
{
public:
	TOW_Map();
	virtual ~TOW_Map();

	IHeightMap* HeightMap();
	virtual RESULT Render();
	void SetDevice(IDevice* device);
	RESULT Load(char* file);
	RESULT Create(int x, int z, float size);
	RESULT Destroy();
	ITextureMap* TextureMap();
	void SetScene(CScene* scene);
	void ToggleCulling();
	bool ViewWithinBounds(CCamera* cam);
	int width();
	int height();
private:
	IDevice* myDevice;
	TOW_HeightMap* m_heightMap;
	TOW_TextureMap* m_textureMap;
	Terrain_Tile* tiles;
	CScene* m_scene;
	int map_cx;
	int map_cz;
	float m_tile_size;
	RECT m_cull_rect;
	bool m_re_calc_culling;
	void CalcCullRect(CCamera* cam, RECT* r);
	std::string m_file;
	mutex m_allow;
};

#endif