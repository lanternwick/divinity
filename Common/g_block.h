#ifndef G_BLOCK_H
#define G_BLOCK_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

template<typename t>
class g_parser;

template<typename t>
class g_deparser;

#include "g_exception.h"

template<typename t>
class g_pair
{
public:
	g_pair(){}
	g_pair(const std::basic_string<t>& tname){name = tname;}
	g_pair(const std::basic_string<t>& tname, const std::basic_string<t> tval)
	{name = tname; value = tval;}
	std::basic_string<t> name;
	std::basic_string<t> value;

	bool operator == (const g_pair& rhs)
	{
		return name == rhs.name;
	}
};

template<typename t>
class g_block
{
	friend class g_parser<t>;
	friend class g_deparser<t>;
public:
	g_block(){}
	~g_block(){}

	void erase(){m_type.clear(); m_attributes.clear(); m_contains.clear(); m_data.clear();}

	std::basic_string<t>& type(){ return m_type;}
	const std::basic_string<t>& type() const {return m_type;}
	std::basic_string<t>& attribute(const std::basic_string<t>& attr_name)
	{
		std::vector<g_pair<t> >::iterator f = 
			std::find(m_attributes.begin(), m_attributes.end(), g_pair<t>(attr_name));
		if(f == m_attributes.end())
			throw g_exception(__FILE__, __LINE__, "No such attribute!");
		return (*f).value;
	}
	// for retrieval only
	std::basic_stringstream<t> operator()(const std::basic_string<t>& attr_name)
	{
		std::basic_stringstream<t> ss;
		ss << attribute(attr_name);
		return ss;
	}
	bool attribute_exists(const std::basic_string<t>& attr_name)
	{
		std::vector<g_pair<t> >::iterator f = 
			std::find(m_attributes.begin(), m_attributes.end(), g_pair<t>(attr_name));
		if(f == m_attributes.end())
			return false;
		return true;
	}
	g_pair<t>& attribute(unsigned int attr_i)
	{
		if(attr_i >= m_attributes.size())
			throw g_exception(__FILE__, __LINE__, "No such attribute!");
		return m_attributes[attr_i];
	}
	unsigned int num_attributes()
	{
		return m_attributes.size();
	}
	std::basic_string<t>& data(){ return m_data; }
	const std::basic_string<t>& data() const {return m_data; }
	unsigned int num_blocks() { return m_contains.size(); }
	unsigned int num_blocks_of_type(const std::basic_string<t>& type_name) const
	{
		unsigned int c = 0;
		for(std::vector<g_block<t> >::const_iterator i = m_contains.begin(); i != m_contains.end(); ++i)
		{
			if((*i).type() == type_name) 
			{
				++c;
			}
		}
		return c;
	}
		
	g_block<t>& block(const std::basic_string<t>& type_name, int block_i=0)
	{
		std::vector<g_block<t> >::iterator i = m_contains.begin();
		
		do
		{
			for(; i != m_contains.end(); ++i)
			{
				if((*i).type() == type_name) 
				{
					break;
				}
			}
			if(i == m_contains.end())
			{
				throw g_exception(__FILE__, __LINE__, "No such block!");
			}
			if(block_i) ++i;
		} while(block_i--);
		return (*i);
	}
	g_block<t>& block(int block_i)
	{
		if(block_i > m_contains.size())
			throw g_exception(__FILE__, __LINE__, "No such block!");
		return m_contains[block_i];
	}
private:
	std::basic_string<t> m_type;
	std::vector<g_pair<t> > m_attributes;
	std::basic_string<t> m_data;
	std::vector<g_block<t> > m_contains;
};

#endif
