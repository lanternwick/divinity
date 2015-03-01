#ifndef _IMESH_H
#define _IMESH_H

#include "..\Common\define_exports.h"

#include "..\Common\IObject.h"
#include "..\Common\IRenderable.h"
#include "..\Common\IThinker.h"

#include "IDevice.h"
#include "IGraphObjFactory.h"
#include "ITexture.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "..\Common\CSmartPtr.h"


#include "Media_Types.h"

class _API IMesh : public IObject, IRenderable, IThinker
{
public:
	IMesh();
	~IMesh();

	virtual RESULT Think()=0;
	virtual RESULT Render()=0;

	virtual RESULT LoadMesh(CSmartPtr<IGraphObjFactory> factory, char* file)=0;
	virtual RESULT ExportMesh(char* file)=0;
	VOID SetDimensions(VECTOR* dim);
	
	VOID SetDevice(CSmartPtr<IDevice> dev);


protected:
	
	CSmartPtr<IVertexBuffer> myVB;
	CSmartPtr<IIndexBuffer> myIB;
	MATERIAL myMat;
	CSmartPtr<IDevice> myDevice;
	VECTOR myDim;

};


#endif

