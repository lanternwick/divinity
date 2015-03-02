#ifndef _CLOG_H
#define _CLOG_H

#include <stdio.h>

class CLog
{
public:
	CLog();
	~CLog();

	virtual VOID OutputData(char* data);
	 CLog* NextRep;
};

CLog::CLog()
{
	NextRep = NULL;
}

CLog::~CLog()
{

}


VOID CLog::OutputData(char* data)
{
	CLog* next = NextRep;
	OutputDebugString(data);
	while(next)
	{
		next->OutputData(data);
		next = next->NextRep;
	}
	return;
}

#endif

	