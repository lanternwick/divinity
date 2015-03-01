#include "IGraphPlugin.h"

char* IGraphPlugin::GetName() const
{
	// get the plugin name
	return pluginName;
}

void IGraphPlugin::SetFactory(CSmartPtr<IGraphObjFactory>& factory)
{
	// set the factory
	myFactory = factory;
	return;
}

void IGraphPlugin::SetDevice(CSmartPtr<IDevice>& dev)
{
	// set the rendering device
	myDevice = dev;
	return;
}