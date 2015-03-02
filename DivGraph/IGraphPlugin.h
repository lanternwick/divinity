#ifndef _IGRAPHPLUGIN_H
#define _IGRAPHPLUGIN_H

#include "IGraphObjFactory.h"
#include "IDevice.h"
#include "..\Common\IObject.h"

class _API IGraphPlugin;



class _API IGraphPlugin
{
public:
	IGraphPlugin(){}
	virtual ~IGraphPlugin() {  }
	virtual IObject* CreateObject(unsigned int id)=0;
	char* GetName() const;
	void SetFactory(IGraphObjFactory* factory);
	void SetDevice(IDevice* dev);
	IGraphPlugin* nextPlugin;
protected:
	char* pluginName;
	IGraphObjFactory* myFactory;
	IDevice* myDevice;
};


#endif

