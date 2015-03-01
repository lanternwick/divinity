#ifndef TOW_GAMEOBJECTTEMPLATE_H
#define TOW_GAMEOBJECTTEMPLATE_H

#include <fstream>
#include <sstream>
#include "property_map.h"
#include "Console\string_tokenizer.h"
#include "..\Common\custmoniker.h"
class TOW_GameObject;

template<class t>
inline void read_line(std::istream& is, std::basic_string<t>& str, t delim='\n')
{
	t c;
	str.erase();
	while(true)
	{
		
		is.read(&c, sizeof(t));
		if(c == delim)
		{
			break;
		}
		if(is.eof()) 
			break;
		str += c;
	}
}

template<class io=std::ifstream>
class TOW_GameObjectTemplate
{
	friend class TOW_GameObject;
public:
	TOW_GameObjectTemplate(){}
	~TOW_GameObjectTemplate(){}

	moniker name()
	{
		moniker m;
		try
		{
			m() = properties.property(std::string("name")).as_if_string();
			return m;
		}
		catch(...)
		{
			return moniker("");
		}
	}

	bool Load(io& stream)
	{
		std::string t;
		//property_map<> properties;
		
		float f;
		long l;
		variant v;
		while(!stream.eof())
		{
			read_line(stream, t);
			if(t == "")
				return true;
			string_tokenizer<> s(t, std::string(" "));

			std::stringstream buff;
			if(s.token(1) == "append")
			{
				v = s.token(2);
				if(properties.property_exists(s.token(0)))
				{
					properties.property(s.token(0)) += s.token(2);
				}
				else
				{
					properties.add_property(s.token(0), s.token(0), &v);
				}
					
			}
			else
			{
				switch(s.token(1)[0])
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					
					buff << s.token(1);
					if(buff.str().find('.')==buff.str().npos)
					{
						buff >> l;
						v = l;
						//v.set_type(string("long"));
						properties.add_property(s.token(0), s.token(0), &v);
					}
					else
					{
						buff >> f;
						v = f;
						properties.add_property(s.token(0), s.token(0), &v);
					}
					break;
				default:
					v = s.token(1);
					properties.add_property(s.token(0), s.token(0), &v);
				}
			}
			
			t = "";
		}
		return true;	
	}	
	variant& property(std::string& prop)
	{
		return properties.property(prop);
	}
	bool property_exists(std::string& prop)
	{
		return properties.property_exists(prop);
	}
protected:
	property_map<> properties;
};

#endif