#ifndef G_OUTPUT_H
#define G_OUTPUT_H

template<typename t>
class g_output
{
public:
	g_output(){}
	virtual ~g_output(){}
	virtual int write(const t& o)=0;
};

template<typename t>
class string_g_output : public g_output<t>
{
public:
	string_g_output(){}
	virtual ~string_g_output(){}
	virtual int write(const t& o)
	{
		m_string.push_back(o);
		return 1;
	}
private:
	std::basic_string<t> m_string;
};

template<typename t>
class file_g_output : public g_output<t>
{
public:
	file_g_output(){m_f = NULL;}
	file_g_output(const std::string& filename)
	{
		m_f = NULL; 
		m_f = std::fopen(filename.c_str(), "w");
	}
	virtual ~file_g_output()
	{
		if(m_f) fclose(m_f);
		m_f = NULL;
	}
	virtual int write(const t& o)
	{
		if(m_f)
		{
			fwrite(&o, sizeof(t), 1, m_f);
			return 0;
		}
		return 1;
	}
private:
	FILE* m_f;
};

#endif
