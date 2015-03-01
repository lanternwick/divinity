#include "log.h"

Log::Log()
{
	buffer = NULL;
	buffer = new char[4096];
	buffer_length = 0;
	max_buffer_size = 4096;
	buffer[0] = 0;
}

Log::~Log()
{
	if(buffer)
		delete buffer;
	buffer_length = 0;
	max_buffer_size = 0;
}

void Log::Write(const char* log_string)
{
	if(!buffer) return;
	unsigned long l = strlen(log_string);
	unsigned long lBuff = strlen(buffer);
	
	char* newBuff = NULL;
	
	char* text1 = new char[strlen(log_string)+1];
	long i;
	long j;
	long text_len = strlen(log_string);
	// fix up backspaces...
	long rstart = 0;
	while(log_string[rstart]=='\b')
		++rstart;
	if(strcmp(log_string, "\b")==0)
	{
		buffer[lBuff-1] = 0;
		return;
	}
	for(j = -1, i = rstart; i < text_len+1; ++i)
	{
		if(log_string[i] == '\b')
		{
			--j;
			continue;
		}
		++j;
		text1[j] = log_string[i];
	}
	text_len = strlen(text1);

	if(l + lBuff > max_buffer_size)
	{
		max_buffer_size += 4096;
		newBuff = new char[max_buffer_size];
		memcpy(newBuff, buffer, l+lBuff);
		delete buffer;
		buffer = newBuff;
	}
	//++buffer_length;
	strcat(buffer, text1);
	buffer_length += l;
	delete text1;
	
}

char* Log::Contents(int num_lines) const
{
	if(num_lines == -1) 
		return buffer;

	char* c = &buffer[strlen(buffer)-1];
	if(*c=='\n')
		--c;
	int count = 0;
	while(c>buffer)
	{
		if(*c=='\n')
			++count;
		if(count == num_lines)
		{
			return ++c;
		}
		--c;
	}

	return buffer;
}

void Log::clear()
{
	buffer[0] = 0;
	buffer_length = 0;
}