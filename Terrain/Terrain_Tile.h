#ifndef _TERRAIN_TILE_H
#define _TERRAIN_TILE_H

#include "..\DivCore\DivCore.h"
#include "..\Common\define_exports.h"
//#include "GraphObjInterface_Plugin.h"
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
#include <stdio.h>



class _API Terrain_Tile : public IObject, IRenderable, IThinker
{
public:
	Terrain_Tile(int numTextures);
	~Terrain_Tile();

	static void SetFactory(CSmartPtr<IGraphObjFactory>& factory);
	static void SetDevice(CSmartPtr<IDevice>& device);

	RESULT Create(VECTOR* corner0, VECTOR* corners, float scale, unsigned char subDivision);

	RESULT Render();
	RESULT Think();

	void Dirty();
	void DrawSimple();
	void DrawComplex();

	//static CSmartPtr<ITexture>* m_Texture;
private:
	Terrain_Tile();
	unsigned char m_ucScale;
	VECTOR corners[4];
	float m_fractalHeight(float x, float z);
	CSmartPtr<IIndexBuffer> m_ib;
	CSmartPtr<IVertexBuffer> m_vb;
	CSmartPtr<IIndexBuffer> m_simpleIB;
	bool m_dirty;
	unsigned long m_verts;
	unsigned long m_indices;
	static CSmartPtr<IGraphObjFactory> m_factory;
	static CSmartPtr<IDevice> m_Device;
	bool m_simple;
};

#endif
	

