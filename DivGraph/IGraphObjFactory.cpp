#include "IGraphObjFactory.h"

VOID IGraphObjFactory::SetDevice(CSmartPtr<IDevice> dev)
{
	// set the device
	myDevice = dev;
	return;
}