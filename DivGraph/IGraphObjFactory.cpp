#include "IGraphObjFactory.h"

VOID IGraphObjFactory::SetDevice(IDevice* dev)
{
	// set the device
	Core::Function_Enter("IGraphObjFactory::SetDevice()");
	myDevice = dev;
	Core::Function_Exit();
	return;
}