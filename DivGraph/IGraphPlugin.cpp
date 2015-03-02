#include "IGraphPlugin.h"

char* IGraphPlugin::GetName() const
{
	// get the plugin name
	Core::Function_Enter("IGraphPlugin::GetName()");
	char* p = pluginName;
	Core::Function_Exit();
	return pluginName;
}

void IGraphPlugin::SetFactory(IGraphObjFactory* factory)
{
	// set the factory
	Core::Function_Enter("IGraphPlugin::SetFactory()");
	myFactory = factory;
	Core::Function_Exit();
	return;
}

void IGraphPlugin::SetDevice(IDevice* dev)
{
	// set the rendering device
	Core::Function_Enter("IGraphPlugin::SetDevice()");
	myDevice = dev;
	Core::Function_Exit();
	return;
}