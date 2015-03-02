#ifndef TOW_PATHINGMAP_H
#define TOW_PATHINGMAP_H

#pragma warning(disable:4786)

#include <vector>
#include <list>
#include <queue>
#include <string>
#include <fstream>
#include "..\Common\Vector.h"
#include "TOW_GameObject.h"
#include "IHeightMap.h"
#include "..\DivCore\DivCore.h"

class TOW_PathingMap
{
public:
	class node
	{
		//friend bool operator > (TOW_PathingMap::node* lhs, TOW_PathingMapnode* rhs);
	public:
		unsigned long num_adjacent;
		VECTOR position;
		TOW_GameObject* contents;
		float weight;
		std::list<unsigned long> adjacency;
		node* parent;
	};
	class in_search_node
	{
	public:
		node* p_node;
		
		float g;
		float h;
		bool operator == (const in_search_node& rhs){
			return (p_node->position.x == rhs.p_node->position.x) && (p_node->position.z == rhs.p_node->position.z);
		}
		bool operator != (const in_search_node& rhs){
			return (p_node->position.x != rhs.p_node->position.x) || (p_node->position.z != rhs.p_node->position.z);
		}
		bool operator > (const in_search_node& rhs){
			return g+h<rhs.g+rhs.h;
		}
		bool operator < (const in_search_node& rhs){
			return g+h>rhs.g+rhs.h;
		}
		bool operator >= (const in_search_node& rhs){
			return g+h<=rhs.g+rhs.h;
		}
		bool operator <= (const in_search_node& rhs){
			return g+h>=rhs.g+rhs.h;
		}

	};

	TOW_PathingMap();
	~TOW_PathingMap();
	
	std::vector<node*> find_path(VECTOR& start_node, VECTOR& end_node);
	void generate_pathing(std::istream& file, IHeightMap* hmap);
private:
	float CostToGoal(const in_search_node& n, const in_search_node& g);
	float CostToStart(in_search_node& n, in_search_node& s);
	float cost(const in_search_node& s, const in_search_node& e);
	std::vector<node*> nodes;
	unsigned long m_map_width;

};


#endif