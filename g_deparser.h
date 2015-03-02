#ifndef G_DEPARSER_H
#define G_DEPARSER_H

#include "g_block.h"
#include "g_output.h"
#include "g_exception.h"

template<typename t>
class g_deparser
{
public:
	g_deparser(){}
	g_deparser(g_output<t>& o, g_block<t>& root) { deparse(o, root); }

	int deparse(g_output<t>& o, g_block<t>& root, int level=0)
	{
		
		if(root.type() != "")
		{
			for(int j = 0; j < level; ++j)
				o.write(static_cast<t>('\t'));
			o.write(static_cast<t>('<'));
			write(o, root.m_type);
			for(std::vector<g_pair<t> >::iterator i = root.m_attributes.begin();
				i != root.m_attributes.end(); ++i)
			{
				o.write(static_cast<t>(' '));
				write(o, (*i).name);
				if((*i).value != "null")
				{
					o.write(static_cast<t>('='));
					o.write(static_cast<t>('\"'));
					write(o, (*i).value);
					o.write(static_cast<t>('\"'));
				}
			}
			if(root.data().size() == 0)
			{
				o.write(static_cast<t>(' '));
				o.write(static_cast<t>('/'));
			}
			o.write(static_cast<t>('>'));
			o.write(static_cast<t>('\n'));
			if(root.data().size())
			{
				for(int j = 0; j < level+1; ++j)
					o.write(static_cast<t>('\t'));
				write(o, root.data(), 70, level+1);
				o.write(static_cast<t>('\n'));
				for(std::vector<g_block<t> >::iterator k = root.m_contains.begin();
					k != root.m_contains.end(); ++k)
				{
					deparse(o, (*k), level+1);
				}
				for(int l = 0; l < level; ++l)
					o.write(static_cast<t>('\t'));
				o.write(static_cast<t>('<'));
				o.write(static_cast<t>('/'));
				write(o, root.type());
				o.write(static_cast<t>('>'));
				o.write(static_cast<t>('\n'));
			}
			o.write(static_cast<t>('\n'));
		}
		else
		{
			for(std::vector<g_block<t> >::iterator k = root.m_contains.begin();
				k != root.m_contains.end(); ++k)
			{
				deparse(o, (*k), level);
			}
		}
		return 0;
	
	}
private:
	void write(g_output<t>& o, std::basic_string<t> s, size_t num=0, size_t level=0)
	{
		if(num == 0) num = s.size();
		size_t c = 0;
		for(std::basic_string<t>::iterator i = s.begin(); i != s.end(); ++i, ++c)
		{
			if(c == num)
			{
				o.write(static_cast<t>('\n'));
				for(unsigned int j = 0; j < level; ++j)
					o.write(static_cast<t>('\t'));
				c = 0;
			}

			if((*i) == static_cast<t>('\t'))
			{
				o.write(static_cast<t>('\\'));
				o.write(static_cast<t>('t'));
				continue;
			}
			if((*i) == static_cast<t>('\n'))
			{
				o.write(static_cast<t>('\\'));
				o.write(static_cast<t>('n'));
				continue;
			}
			if((*i) == static_cast<t>('\\'))
			{
				o.write(static_cast<t>('\\'));
				o.write(static_cast<t>('\\'));
				continue;
			}
			if(i != s.begin())
			{
				if((*i) == static_cast<t>(' '))
				{
					if((*(i-1)) != static_cast<t>(' '))
					{
						o.write(*i);
						continue;
					}
					else
					{
						o.write(static_cast<t>('\\'));
						o.write(*i);
						continue;
					}
				}
			}
			o.write(*i);
		}
	}
};

#endif
