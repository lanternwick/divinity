#include "IVertexBuffer.h"
#include "IIndexBuffer.h"
#include "..\Common\CSmartPtr.h"

#include "Imesh.h"



IMesh::IMesh()
{


}

IMesh::~IMesh()
{
	// cleanup
	myVB.Release();
	myIB.Release();
}

VOID IMesh::SetDevice(CSmartPtr<IDevice> dev)
{
	// set the device
	myDevice = dev;
	return;
}

VOID IMesh::SetDimensions(VECTOR* dim)
{
	// set dimensions
	myDim = *dim;
	return;
}