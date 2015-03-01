#ifndef _STATICMESH_H
#define _STATICMESH_H

#include "IMesh.h"
#include "..\Common\types.h"
#include "..\Common\FileGuard.h"
#include "..\Common\matrix.h"
#include <vector>

struct mesh_subset
{
	unsigned long numVerts;
	unsigned long numIndices;
	unsigned long numFaces;
	MATERIAL mat;
	IVertexBuffer* vb;
	IIndexBuffer* ib;
	MATRIX scale;
};

struct mesh
{
	unsigned long numSubsets;
	std::vector<mesh_subset> subsets;
};

class _API StaticMesh : public IMesh
{
public:
	StaticMesh(char* id);
	virtual ~StaticMesh();

	RESULT Think();
	RESULT Render();

	RESULT LoadMesh(IGraphObjFactory* factory, char* file);
	RESULT ExportMesh(char* file);

	void DrawSubset(unsigned long subset);
private:
	StaticMesh();
	BOOL indexed;
	mesh m_mesh;
	MATRIX scale;
};



#endif