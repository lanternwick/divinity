#ifndef G_EXCEPTION_H
#define G_EXCEPTION_H

#include <string>
#include <sstream>

class g_exception
{
public:
	g_exception(){}
	g_exception(const char* file_, size_t line_, const char* error_=NULL)
	{
		if(file_)
			m_file = file_;
		m_line = line_;
		if(error_)
		{
			m_error = error_;
		}
		else
		{
			m_error = "none";
		}
	}
	g_exception(const g_exception& e)
	{
		m_file = e.m_file;
		m_line = e.m_line;
		m_error = e.m_error;
	}
	~g_exception(){}
	const std::string format_e() const
	{
		std::stringstream ss;
		ss << "Exception raised in file " << m_file << ":" << static_cast<unsigned int>(m_line) << ".\nError reported: "
			<< m_error;
		return ss.str();
	}
	const std::string& file() const
	{
		return m_file;
	}
	const std::string& error() const
	{
		return m_error;
	}
	const size_t line() const
	{
		return m_line;
	}
private:
	std::string m_file;
	std::string m_error;
	size_t m_line;
};

#endif
