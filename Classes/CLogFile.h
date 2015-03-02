#ifndef _CLOGFILE_H
#define _CLOGFILE_H

#include <CLog.h>

class CLogFile : public CLog
{
public:
	CLogFile();
	CLogFile(char* file);
	~CLogFile();

	VOID OutputData(char* data);
	VOID Open(char* file);
	VOID Reset();
private:
	FILE* f;
};

CLogFile::CLogFile()
{
	f = NULL;
}

CLogFile::CLogFile(char* file)
{
	f = NULL;
	Open(file);
}

CLogFile::~CLogFile()
{
	fclose(f);
}

VOID CLogFile::OutputData(char* data)
{
	if(f)
		fputs(data, f);
	return;
}

VOID CLogFile::Open(char* file)
{
	f = fopen(file, "w+");
	return;
}

VOID CLogFile::Reset()
{
	if(f)
		fseek(f, 0, SEEK_SET);
}

#endif