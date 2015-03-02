#ifndef _IMESH_H
#define _IMESH_H

#include "..\Common\define_exports.h"

#include "..\Common\IObject.h"
#include "..\Common\IEntity.h"


#include "IDevice.h"
#include "IGraphObjFactory.h"
#include "ITexture.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"



#include "Media_Types.h"

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

class _API IMesh : public IMediaObject
{
public:
	IMesh(char* id);
	virtual ~IMesh();

	virtual RESULT Think()=0;
	virtual RESULT Render()=0;

	virtual RESULT LoadMesh(IGraphObjFactory* factory, char* file)=0;
	virtual RESULT ExportMesh(char* file)=0;
	VOID SetDimensions(VECTOR* dim);
	
	VOID SetDevice(IDevice* dev);

	virtual void DrawSubset(unsigned long subset)=0;

protected:
	IMesh();
	IVertexBuffer* myVB;
	IIndexBuffer* myIB;
	MATERIAL myMat;
	IDevice* myDevice;
	VECTOR myDim;

};


#endif

