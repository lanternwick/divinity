#include "IGraphPlugin.h"

char* IGraphPlugin::GetName() const
{
	return pluginName;
}

void IGraphPlugin::SetFactory(IGraphObjFactory* factory)
{
	myFactory = factory;
	return;
}
