#ifndef G_PARSER_H
#define G_PARSER_H

#include "g_block.h"
#include "g_input.h"
#include "g_exception.h"
#include <list>
#include <tchar.h>

template<typename t>
class g_parser
{
public:
	g_parser(){}
	g_parser(g_input<t>& i){parse(i);}

	g_block<t>& root(){return m_root;}

	g_block<t>& parse(g_input<t>& i)
	{
		m_root.erase();
		std::list<g_block<t>*> cb;
		cb.push_back(&m_root);
		g_pair<t> attr;
		t it;
		int state = 0;
		std::basic_string<t> type_e;
		bool attr_p2 = false;
		bool in_quote = false;
		g_block<t> new_block;
		bool am_escaping = false;
		bool was_whitespace = false;
		bool in_tag_end = false;
		while(!i.eos())
		{
			i.read(it);
			switch(state)
			{
			case 0: // empty state
				if(in_tag_end)
				{
					if(it == static_cast<t>('>'))
					{
						in_tag_end = false;
						cb.pop_back();
					}
					break;
				}
				if(am_escaping)
				{
					if(it == static_cast<t>('t')) // tab char
					{
						(*cb.back()->m_data.rbegin()) = static_cast<t>('\t');
						
					}
					if(it == static_cast<t>('n')) // newline
					{
						(*cb.back()->m_data.rbegin()) = static_cast<t>('\n');
					}
					if(it == static_cast<t>('\\'))
					{
						(*cb.back()->m_data.rbegin()) = static_cast<t>('\\');
					}
					if(it == static_cast<t>('<'))
					{
						(*cb.back()->m_data.rbegin()) = static_cast<t>('<');
					}
					if(it == static_cast<t>(' '))
					{
						(*cb.back()->m_data.rbegin()) = static_cast<t>(' ');
					}
					am_escaping = false;
					break;
				}
				else
				{
					if(it == static_cast<t>('<'))
					{ // a tag
						state = 1;
					}
					else
					{
						if(it == static_cast<t>('\\'))
						{
							am_escaping = true;
							cb.back()->m_data.push_back(it);
							was_whitespace = false;
							break;
						}
						if((it == static_cast<t>(' ')) || 
							(it == static_cast<t>('\t')))
						{
							if(was_whitespace)
							{
								break;
							}
							else
							{
								cb.back()->m_data.push_back(static_cast<t>(' '));
								was_whitespace = true;
								break;
							}
						}
						cb.back()->m_data.push_back(it);
						was_whitespace = false;
					}
					
					
				}
				break;
			case 1: // determine new tag or end tag
				if(it == static_cast<t>('/'))
				{
					state = 3;
					break;
				}
				else
				{
					state = 2;
					cb.back()->m_contains.push_back(new_block);
					cb.push_back(&(cb.back()->m_contains.back()));
				}
			case 2: // new tag, read type
				if((it == static_cast<t>(' ')) || 
					(it == static_cast<t>('\t')))
				{
					state = 4;
					break;
				}
				if(it == static_cast<t>('>'))
				{
					state = 0;
					break;
				}
				cb.back()->m_type.push_back(it);
				break;
			case 3: // ending a tag
				if(it == static_cast<t>('>'))
				{
					if(type_e == cb.back()->m_type)
					{
						cb.pop_back();
						state = 0;
						type_e.clear();
						break;
					}
					else
					{
						std::stringstream ss;
						ss << "Tag mismatch! Starting tag type is " << cb.back()->m_type;
						ss << "\nEnding tag type is " << type_e << "!" << std::endl;
						throw g_exception(__FILE__, __LINE__, ss.str().c_str()); throw; throw;
					}
				}
				type_e.push_back(it);
				break;
			case 4: // read attributes
				if((it == static_cast<t>(' ')) || 
					(it == static_cast<t>('\t')))
				{
					if(!attr_p2)
					{
						break;
					}
					else
					{
						if(in_quote)
						{
							attr.value.push_back(it);
							break;
						}
						else
						{
							if(attr.value.size() == 0)
							{
								break;
							}
							else
							{
								cb.back()->m_attributes.push_back(attr);
								attr.name.clear();
								attr.value.clear();
								attr_p2 = false;
								break;
							}
						}
					}
				}
				if(it == static_cast<t>('/'))
				{
					if(attr.name.size())
					{
						if(!attr_p2)
						{
							attr.value = reinterpret_cast<t*>("null");
						}
						cb.back()->m_attributes.push_back(attr);
						attr.name.clear();
						attr.value.clear();
						in_quote = false;
						attr_p2 = false;
					}
					in_tag_end = true;
					state = 0;
					break;
				}

				if(it == static_cast<t>('>'))
				{
					if(!attr.name.size())
					{
						state = 0;
						break;
					}
					if(!attr.value.size())
					{
						attr.value = reinterpret_cast<t*>("null");
					}
					cb.back()->m_attributes.push_back(attr);
					attr.name.clear();
					attr.value.clear();
					attr_p2 = false;
					in_quote = false;
					state = 0;
					break;
				}
				if(it == static_cast<t>('='))
				{
					attr_p2 = true;
					break;
				}
				if(it == static_cast<t>('\"'))
				{
					if(attr_p2)
					{
						if((attr.value.size()) && (*attr.value.rbegin()) == static_cast<t>('\\'))
						{
							(*attr.value.rbegin()) = static_cast<t>('\"');
							break;
						}
						else
						{
							if(in_quote)
							{
								cb.back()->m_attributes.push_back(attr);
								attr.name.clear();
								attr.value.clear();
								attr_p2 = false;
								in_quote = false;
							}
							else
							{
								in_quote = true;
							}
							break;
						}
					}
				}
				if(attr_p2)
				{
					attr.value.push_back(it);
				}
				else
				{
					attr.name.push_back(it);
				}
				break;
			}
		}
		return m_root;
	}
private:
	g_block<t> m_root;
};

#endif;
	