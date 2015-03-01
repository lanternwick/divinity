#ifndef _LOG_H
#define _LOG_H

#include "..\Common\define_exports.h"
#include <string.h>


class _API Log
{
public:
	Log();
	~Log();

	void Write(const char* log_string);
	char* Contents(int num_lines) const;
	void clear();
private:
	char* buffer;
	unsigned long buffer_length;
	unsigned long max_buffer_size;
};

#endif