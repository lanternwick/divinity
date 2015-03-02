#ifndef MAP_LOAD_THREAD_H
#define MAP_LOAD_THREAD_H

#include "..\DivCore\thread.h"
#include "TOW_Map.h"
#include <string>

class map_load_thread : public thread
{
public:
	map_load_thread();
	map_load_thread(TOW_Map* map, std::string& filename);
private:
	virtual void run_thread();
	TOW_Map* m_map;
	std::string m_filename;
};

#endif
