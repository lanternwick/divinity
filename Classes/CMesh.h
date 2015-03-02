#ifndef _CMESH_H
#define _CMESH_H

#include <d3dx8.h>
#include <CObject.h>

class CMesh : public CObject
{
public:
	CMesh()
	{
		mesh = NULL;
		type = OBJ_MESH;
	}
	
	~CMesh()
	{
		if(mesh != NULL) mesh->Release();
	}


	LPD3DXMESH mesh;
};

#endif