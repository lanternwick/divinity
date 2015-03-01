#ifndef _GEOSTATICMESH_H
#define _GEOSTATICMESH_H

#include "IMesh.h"
#include <types.h>

class StaticMesh : public IMesh
{
public:
	StaticMesh();
	~StaticMesh();

	RESULT Update();
	RESULT Render(DWORD& numPrims);

	RESULT LoadMesh(char* file);
	RESULT ExportMesh(char* file);
private:
	BOOL indexed;
	DWORD numI;
	DWORD numV;
};

#endif