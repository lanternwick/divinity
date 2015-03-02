#ifndef _IMESH_H
#define _IMESH_H


#include <CObject.h>
#include <FileGuard.h>
#include <MemGuard.h>

#define ID_SKINNEDMESH (0xe29a7735)
#define ID_STATICMESH  (0xb83e9c38)


class IMesh : public CObject
{
public:
	IMesh();
	~IMesh();

	RESULT Update();
	RESULT Render(DWORD& numPrims);

	RESULT LoadMesh(char* file);
	RESULT ExportMesh(char* file);
	
protected:
	HANDLE VB;
	HANDLE IB;
	MATERIAL myMat;

};

IMesh::IMesh()
{
	type = OBJ_MESH;
}

IMesh::~IMesh()
{
	DeleteObj(VB);
	DeleteObj(IB);
}

RESULT IMesh::Update()
{
	return CObject::Update();
}

RESULT IMesh::Render(DWORD& numPrims)
{

	return OK;
}

RESULT IMesh::LoadMesh(char* file)
{

	return OK;
}

RESULT IMesh::ExportMesh(char* file)
{

	return OK;
}

#endif

