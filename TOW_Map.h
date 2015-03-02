#ifndef _TOW_MAP_H
#define _TOW_MAP_H

#include "IHeightMap.h"
#include "TOW_HeightMap.h"
#include "TOW_TextureMap.h"
#include "Terrain_Tile.h"
#include <vector>
#include  "mutex.h"
#include "IObject.h"

class TOW_Map : public IObject
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
	void SetCamera(CCamera* cam);
	void ToggleCulling();
	bool ViewWithinBounds(CCamera* cam);
	int width();
	int height();
	void ToggleShadows();
private:
	IDevice* myDevice;
	TOW_HeightMap* m_heightMap;
	TOW_TextureMap* m_textureMap;
	Terrain_Tile* tiles;
	CCamera* m_camera;
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