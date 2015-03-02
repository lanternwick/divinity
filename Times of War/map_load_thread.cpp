#include "map_load_thread.h"

map_load_thread::map_load_thread()
{

}

map_load_thread::map_load_thread(TOW_Map* map, std::string& filename)
{
	m_map = map;
	m_filename = filename;
}

void map_load_thread::run_thread()
{	
//	m_mutex.lock();
	m_map->Load((char*)m_filename.c_str());
	m_map->Create(0,0,0.0f);
//	m_mutex.unlock();
}