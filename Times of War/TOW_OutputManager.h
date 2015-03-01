#ifndef TOW_OUTPUTMANAGER_H
#define TOW_OUTPUTMANAGER_H

#include "TOW_GameOutput.h"
#include <list>

template<class t=TOW_GameOutput>
class TOW_OutputManager
{
public:
	~TOW_OutputManager();
	t* InsertObject(t* oo);
private:
	std::list<t*> objects;
};

#include "TOW_OutputManager.inc"

#endif