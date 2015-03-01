#ifndef _TERRAIN_TILE_H
#define _TERRAIN_TILE_H

#include "..\DivCore\DivCore.h"
#include "..\Common\define_exports.h"
//#include "GraphObjInterface_Plugin.h"
#include "..\DivGraph\DivGraph.h"
#include "..\DivGraph\IGraphObjFactory.h"
#include "..\DivGraph\IDevice.h"

inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}



#include "..\Common\IObject.h"
#include "..\Common\IRenderable.h"
#include "..\Common\IThinker.h"
#include "..\Common\types.h"
#include "..\Common\fvf.h"
#include "IHeightMap.h"
#include <stdio.h>



class _API Terrain_Tile : public IObject, public IThinker
{
public:
	Terrain_Tile();
	virtual ~Terrain_Tile();

	static void SetFactory(IGraphObjFactory* factory);
	static void SetDevice(IDevice* device);
	static void SetHeightMap(IHeightMap* hMap);
	
	RESULT Create(float x, float z, float scale, unsigned char subDivision);

	RESULT Render();
	RESULT Think();

	void Dirty();
	
	void SetTileID(unsigned long newID);
	unsigned long GetTileID();

	bool IsVisible(CCamera* cam);

	//static CSmartPtr<ITexture>* m_Texture;
private:
	
	unsigned char m_ucScale;
	
	static IHeightMap* m_heightMap;
	IIndexBuffer* m_indices_;
	IVertexBuffer* m_vertices;
	IVertexBuffer* m_vertices2;
	bool m_dirty;
	unsigned long m_verts;
	unsigned long m_indices;
	static IGraphObjFactory* m_factory;
	static IDevice* m_Device;
	unsigned long m_tileID;

	float m_scale;
	float m_x;
	float m_z;
};

#endif
	

