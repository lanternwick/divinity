#ifndef TOW_TEMPLATEMANAGER_H
#define TOW_TEMPLATEMANAGER_H

#pragma warning(disable:4786)

#include "..\DivCore\Core.h"
#include "TOW_GameObjectTemplate.h"
#include "..\Common\custmoniker.h"


//#include "TOW_GameUnit.h"

#include <vector>

class TOW_TemplateManager
{
public:
	
	//static moniker CreateObject(moniker& objTemplate, moniker& objName);
	static void LoadTemplateFromFile(std::string& filename);
	static void LoadTemplate(std::string& template_name);
	static TOW_GameObjectTemplate<>* GetTemplate(moniker& template_name);
	static void Clear(bool free_all=false);
	
private:
	static std::vector<TOW_GameObjectTemplate<>* > templates;
	
	
};


#endif