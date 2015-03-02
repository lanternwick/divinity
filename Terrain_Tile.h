#ifndef _TERRAIN_TILE_H
#define _TERRAIN_TILE_H

//#include "GraphObjInterface_Plugin.h"

#include "IDevice.h"

/*inline void* operator new(size_t size)
{
	return GimmeMem(size);
}

inline void operator delete(void* p)
{
	FreeMem(p);
	return;
}*/




#include "CCamera.h"
#include "types.h"
#include "fvf.h"
#include "IHeightMap.h"
#include <stdio.h>
#include "ITextureMap.h"


class Terrain_Tile
{
public:
	Terrain_Tile();
	virtual ~Terrain_Tile();

	static void SetDevice(IDevice* device);
	static void SetHeightMap(IHeightMap* hMap);
	static void SetTextureMap(ITextureMap* tMap);

	
	RESULT Create(float x, float z, float scale, unsigned char subDivision);

	RESULT Render();
	RESULT Think();

	void Dirty();
	
	void SetTileID(unsigned long newID);
	unsigned long GetTileID();

	bool IsVisible(CCamera* cam);
	void ToggleShadows();
	//static CSmartPtr<ITexture>* m_Texture;
private:
	
	struct tri_set
	{
		IIndexBuffer* ib;
		char tid1;
		char tid2;
		void* buffer;
		unsigned long numI;
		tri_set()
		{
			ib = NULL;
			buffer = NULL;
		}
	};
	struct tri_set_init
	{
		std::vector<unsigned short> ind;
		char tid1;
		char tid2;
		unsigned long id;
	};

	unsigned char m_ucScale;
	
	static IHeightMap* m_heightMap;
	static ITextureMap* m_textureMap;
	IIndexBuffer* m_indices_;
	std::vector<tri_set> m_tris;
	IVertexBuffer* m_vertices;
	IVertexBuffer* m_vertices2;
	bool m_dirty;
	unsigned long m_verts;
	unsigned long m_indices;
	static IDevice* m_Device;
	unsigned long m_tileID;

	float m_scale;
	float m_x;
	float m_z;

	bool m_draw_shadows;
};

#endif
	

