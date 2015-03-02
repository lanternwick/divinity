#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <iostream>
#include <sstream>
#include <fstream>

using std::ostream;

#ifdef _MT
#include "mutex.h"
#endif

class _log
{
public:
	_log(const char* out="log.txt")
	{
		m_f = out;
	}
	~_log()
	{
#ifdef _MT
		m_m.lock();
#endif	
		std::ofstream f(m_f.c_str());
		f << m_log.str();
		f.close();
#ifdef _MT
			m_m.unlock();
#endif
	}
	int write(const char* l, int debug_level)
	{
#ifdef _MT
		m_m.lock();
#endif
		if(debug_level<=m_debug_level)
		{
			if((m_log.str()[m_log.str().length()-1] == '\n') || (m_log.str().length() == 0))
				m_log << debug_level << ": ";
			m_log << l;
#ifdef _CONSOLE
			std::cout << l;
#endif
#ifdef _MT
			m_m.unlock();
#endif
			return 0;
		}
#ifdef _MT
			m_m.unlock();
#endif
		return 1; // was not written
	}
	int write(const std::string& l, int debug_level)
	{
		return write(l.c_str(), debug_level);
	}
	int read(std::string& o)
	{
#ifdef _MT
		m_m.lock();
#endif
		o = m_log.str();
#ifdef _MT
			m_m.unlock();
#endif
		return 0;
	}
	int tail(std::string& o, int lines, int offset=0)
	{
		char c;
		long i = m_log.str().length()-1;
		o.erase();
		if(m_log.str()[i] == '\n')
			++offset;
		while((offset) && (i))
		{
			if(m_log.str()[i] == '\n')
				--offset;
			--i;
		}
		while((lines) && (i+1))
		{
			c = m_log.str()[i];
			o = c + o;
			if(c == '\n')
				--lines;
			--i;
		}
		return 0;
	}
	int set_debug_level(int debug_level)
	{
		m_debug_level = debug_level;
		return 0;
	}
private:
	std::stringstream m_log;
	std::string m_f;
	int m_debug_level;
#ifdef _MT
	mutex m_m;
#endif
};


template<long debug_level>
class log_buff : public std::streambuf
{
public:	
	log_buff(_log& l)
		: m_log(l)
	{}
protected:
	log_buff()
	{}
	_log& m_log;
	virtual int overflow(int ch)
	{
		if(ch != 10)
		{
			char b[2] = {0,0};
			b[0] = ch;
			m_log.write(b, debug_level);
		}
		else
		{
			char b[2] = {0,0};
			b[0] = '\n';
			m_log.write(b, debug_level);
		}
		return ch;
	}
};

template<long debug_level>
class log_stream : public ostream
{
private:
	log_stream()
		: ostream(new log_buff<debug_level>)
	{}
public:
	log_stream(_log& l)
		: ostream(new log_buff<debug_level>(l))
	{}
	~log_stream()
	{
		delete rdbuf();
	}
};

#endif
			
