#ifndef PROPERTY_MAP_H
#define PROPERTY_MAP_H

#pragma warning(disable : 4786)


#include <map>
#include <algorithm>
#include "..\Common\variant.h"



inline std::ostream& operator << (std::ostream& lhs, variant& rhs)
{
	if(rhs.type() == std::string("int"))
	{
		lhs << rhs.as_if_int();
	}
	if(rhs.type() == std::string("long"))
	{
		lhs << rhs.as_if_long();
	}
	if(rhs.type() == std::string("float"))
	{
		lhs << rhs.as_if_float();
	}
	if(rhs.type() == std::string("string"))
	{
		lhs << rhs.as_if_string();
	}
	return lhs;
}

template<class cont=variant>
class property_map
{
private:
	std::map<std::string, cont> properties;
	typedef std::map<std::string, cont>::iterator map_it;
public:
	property_map(){add_property(std::string("default"), std::string("string"));}
	~property_map(){}

	void add_property(std::string& prop_name, std::string& type, cont* default_val=0)
	{
		cont c;
		if(default_val)
		{
			c = *default_val;
		}
		else
		{
			c.set_type(type);
		}
		if(properties.find(prop_name)== properties.end())
		{
			std::pair<map_it, bool> ret;
			std::pair<std::string, cont> ins;
			map_it i;
			ins.first = prop_name;
			ins.second = c;
			ret = properties.insert(ins);
			if(ret.second)
			{
				i = ret.first;
				(*i).second = c;
			}
		}
	}
	bool property_exists(std::string prop_name)
	{
		map_it ret;
		
		ret = properties.find(prop_name);
		if(ret==properties.end())
		{
			return false;
		}
		return true;
	}
	property_map<cont>& operator=(property_map<cont>& rhs)
	{
		properties = rhs.properties;
		return *this;
	}
	cont& property(std::string& prop_name)
	{
		map_it ret;
		ret = properties.find(prop_name);
		if(ret==properties.end())
		{
			ret = properties.find(std::string("default"));
		}
		return (*ret).second;
	}
	void output(std::ostream& out)
	{
		map_it i;
		for(i = properties.begin(); i != properties.end(); i++)
		{
			out << (*i).first << ": " << (*i).second << std::endl;
		}
	}
	
};

//#pragma warning(pop)

#endif