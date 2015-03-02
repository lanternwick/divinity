#include "TOW_PathingMap.h"
#include "..\DivCore\Core.h"

TOW_PathingMap::TOW_PathingMap()
{

}

TOW_PathingMap::~TOW_PathingMap()
{

}

float TOW_PathingMap::CostToStart(TOW_PathingMap::in_search_node& n, TOW_PathingMap::in_search_node& s)
{
	return 0.0f;
}

float TOW_PathingMap::CostToGoal(const TOW_PathingMap::in_search_node& n, const TOW_PathingMap::in_search_node& g)
{
	return (float)sqrt((((n.p_node->position.x - g.p_node->position.x)*(n.p_node->position.x - g.p_node->position.x))+
			((n.p_node->position.z - g.p_node->position.z)*(n.p_node->position.z - g.p_node->position.z))));

}

float TOW_PathingMap::cost(const TOW_PathingMap::in_search_node& s, const TOW_PathingMap::in_search_node& e)
{
	float r = s.p_node->weight + e.p_node->weight+(e.p_node->position.y-s.p_node->position.y);
	if(r < 0.0f) 
		return  0.0f;
	return r;
}

std::vector<TOW_PathingMap::node*> TOW_PathingMap::find_path(VECTOR& start, VECTOR& end)
{
	std::vector<node*> q;
	std::vector<in_search_node> open;
	std::vector<in_search_node> closed;
	unsigned long nodex, nodey;
	nodex = (unsigned long)start.x;
	nodey = (unsigned long)start.z;
	if((nodex >= m_map_width) || (nodey >= m_map_width))
		return q;
	node* s_node = nodes[nodex*m_map_width+nodey];
	nodex = (unsigned long)end.x;
	nodey = (unsigned long)end.z;
	if((nodex >= m_map_width) || (nodey >= m_map_width))
		return q;
	node* e_node = nodes[nodex*m_map_width+nodey];
	in_search_node start_node;
	in_search_node goal_node;
	goal_node.p_node = e_node;
	start_node.p_node = s_node;
	start_node.g = 0.0f;
	//goal_node.g = CostToStart(goal_node, start_node);
	start_node.h = CostToGoal(start_node, goal_node);
	goal_node.h = 0.0f;
	start_node.p_node->parent = NULL;

	open.push_back(start_node);
	in_search_node node_c;
	//unsigned long node_i;
	std::list<unsigned long>::iterator i;

	float newg;
	bool in_closed;
	bool in_open;
	while(!open.empty())
	{
		node_c = open.back();
		
		if(node_c == goal_node)
			break;
		open.pop_back();
		for(i = node_c.p_node->adjacency.begin(); i != node_c.p_node->adjacency.end(); i++)
		{
			in_closed = false;
			in_open = false;
			in_search_node temp;
			temp.p_node = nodes[(*i)];
			
			newg = node_c.g + cost(node_c, temp);
			
			std::vector<in_search_node>::iterator found = std::find(closed.begin(), closed.end(), temp);
			
			if(found != closed.end())
			{
				in_closed = true;
				if((*found).g <= newg)
				{
					continue;
				}
			}
			found = std::find(open.begin(), open.end(), temp);
			if(found != open.end())
			{
				in_open = true;
				if((*found).g <= newg)
				{
					continue;
				}
			}
			
			temp.g = newg;
			temp.h = CostToGoal(temp, goal_node);
			temp.p_node->parent = node_c.p_node;
			if(in_closed)
			{
				found = std::remove(closed.begin(), closed.end(), temp);
				closed.resize(closed.size() - (closed.end()-found));
			}
			if(in_open)
			{
				found = std::remove(open.begin(), open.end(), temp);
				open.resize(open.size() - (open.end()-found));	
			}
			open.push_back(temp);
			std::sort(open.begin(), open.end());
		}
		closed.push_back(node_c);
	}
	
	node* s = node_c.p_node;
	while(s->parent)
	{
		q.push_back(s);
		s = s->parent;
	}
	//q.push_back(s);
	std::reverse(q.begin(), q.end());
	return q;
}

void TOW_PathingMap::generate_pathing(std::istream& file, IHeightMap* hmap)
{
	unsigned long numNodes;
	file.read((char*)&numNodes, sizeof(unsigned long));
	file.read((char*)&m_map_width, sizeof(unsigned long));
	nodes.resize(numNodes);
	std::vector<node*>::iterator i;
	std::stringstream ss;
	for(i = nodes.begin(); i != nodes.end(); i++)
	{
		(*i) = new node;
		(*i)->contents = NULL;
		file.read((char*)&((*i)->num_adjacent), sizeof(unsigned long));
		file.read((char*)&((*i)->position.x), sizeof(float));
		(*i)->position.x += 0.5f;
		file.read((char*)&((*i)->position.z), sizeof(float));
		(*i)->position.z += 0.5f;
		(*i)->position.y = hmap->GetHeight((*i)->position.x, (*i)->position.z);
		file.read((char*)&((*i)->weight), sizeof(float));
	}
	unsigned long adjacent_index;
	for(i = nodes.begin(); i != nodes.end(); i++)
	{
		for(unsigned long j = 0; j < (*i)->num_adjacent; ++j)
		{
			file.read((char*)&adjacent_index, sizeof(unsigned long));
			(*i)->adjacency.push_back(adjacent_index);		
		}
	}	
}
