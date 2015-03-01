#ifndef _GRAPHPLUGIN_H
#define _GRAPHPLUGIN_H

#include <IGraphObjFactory.h>

class GraphPlugin
{
public:
	GraphPlugin(){};
	virtual ~GraphPlugin() {};
	virtual WorldObj* CreateObject(unsigned int id);
	char* GetName() const;
protected:
	char* pluginName;
};

#endif

