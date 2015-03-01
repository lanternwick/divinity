#ifndef _STATICMESH_H
#define _STATICMESH_H

#include "IMesh.h"
#include "..\Common\types.h"
#include "..\Common\FileGuard.h"
#include "..\Common\matrix.h"

class _API StaticMesh : public IMesh
{
public:
	StaticMesh();
	~StaticMesh();

	RESULT Think();
	RESULT Render();

	RESULT LoadMesh(CSmartPtr<IGraphObjFactory> factory, char* file);
	RESULT ExportMesh(char* file);
private:
	BOOL indexed;
	DWORD numI;
	DWORD numV;
	DWORD numF;
	MATRIX scale;
};

_EXTERN_ template class _API CSmartPtr<StaticMesh>;

#endif