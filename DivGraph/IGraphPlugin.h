#ifndef _IGRAPHPLUGIN_H
#define _IGRAPHPLUGIN_H

#include "IGraphObjFactory.h"
#include "IDevice.h"
#include "..\Common\IObject.h"

class _API IGraphPlugin;

_EXTERN_ template class _API CSmartPtr<IGraphPlugin>;

class _API IGraphPlugin
{
public:
	IGraphPlugin(){}
	virtual ~IGraphPlugin() {  }
	virtual IObject* CreateObject(unsigned int id)=0;
	char* GetName() const;
	void SetFactory(CSmartPtr<IGraphObjFactory>& factory);
	void SetDevice(CSmartPtr<IDevice>& dev);
	IGraphPlugin* nextPlugin;
protected:
	char* pluginName;
	CSmartPtr<IGraphObjFactory> myFactory;
	CSmartPtr<IDevice> myDevice;
};


#endif

