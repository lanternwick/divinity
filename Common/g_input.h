#ifndef G_INPUT_H
#define G_INPUT_H

#include <string>
#include <cstdio>

template<typename t>
class g_input
{
public:
	g_input(){}
	virtual ~g_input(){};
	// pre: input must be valid
	// post: r != newline, stream has been advanced
	// ret: returns 0 if success, 1 if invalid call
	virtual int read(t& r)=0;
	// pre: none
	// post: none
	// ret: true if end of stream, or stream is not initialized
	virtual bool eos()=0;
};

template<typename t>
class string_g_input : public g_input<t>
{
public:
	string_g_input(){}
	string_g_input(std::basic_string<t>& in){m_string=in; m_c=0;}
	virtual ~string_g_input(){}
	virtual int read(t& r){do{r = m_string[m_c++];}while(r == static_cast<t>('\n'));return 0;}
	virtual bool eos(){return m_c>=m_string.size();}
private:
	std::basic_string<t> m_string;
	unsigned long m_c;
};

template<typename t>
class file_g_input : public g_input<t>
{
public:
	file_g_input(){m_f = NULL;}
	file_g_input(const std::string& filename){m_f = NULL; m_f = fopen(filename.c_str(), "r");}
	virtual ~file_g_input(){if(m_f) fclose(m_f); m_f = NULL;}
	virtual int read(t& r)
	{
		if(m_f)
		{
			do{
				if(eos())
					return 1;
				fread(&r, sizeof(t), 1, m_f);
			} while((r==static_cast<t>('\n')));
			return 0;
		}
		return 1; 
	}
	virtual bool eos() { if(m_f) return (feof(m_f) != 0); return true;}
private:
	FILE* m_f;
};

#endif
