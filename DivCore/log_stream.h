#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <iostream>
#include "Core.h"

using std::ostream;

template<long debug_level>
class log_buff : public std::streambuf
{
public:
	log_buff()
	{}

protected:
	virtual int overflow(int ch)
	{
		if(ch != 10)
		{
			char b[2] = {0,0};
			b[0] = ch;
			Core::WriteLog(b, debug_level);
		}
		else
		{
			char b[2] = {0,0};
			b[0] = '\n';
			Core::WriteLog(b, debug_level);
		}
		return ch;
	}
};

template<long debug_level>
class log_stream : public std::ostream
{
public:
	log_stream()
		: ostream(new log_buff<debug_level>)
	{}
	~log_stream()
	{
		delete rdbuf();
	}
};

#endif
			
