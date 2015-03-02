#include "IVertexBuffer.h"
#include "IIndexBuffer.h"


#include "Imesh.h"



IMesh::IMesh()
{
	Core::Function_Enter("IMesh::Constructor()");
	Core::Function_Exit();

}

IMesh::IMesh(char* id)
{
	Core::Function_Enter("IMesh::Constructor()");
	m_id() = id;
	Core::Function_Exit();
}

IMesh::~IMesh()
{
	// cleanup
	Core::Function_Enter("IMesh::Destructor()");
	if(myVB)
	{
		myVB->Release();
		delete myVB;
	}
	if(myIB)
	{
		myIB->Release();
		delete myIB;
	}
	Core::Function_Exit();
}

VOID IMesh::SetDevice(IDevice* dev)
{
	// set the device
	Core::Function_Enter("IMesh::SetDevice()");
	myDevice = dev;
	Core::Function_Exit();
	return;
}

VOID IMesh::SetDimensions(VECTOR* dim)
{
	// set dimensions
	Core::Function_Enter("IMesh::SetDimensions()");
	myDim = *dim;
	Core::Function_Exit();
	return;
}