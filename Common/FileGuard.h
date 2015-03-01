#ifndef _FILEGUARD_H
#define _FILEGUARD_H

#include <stdio.h>

class FileGuard
{
public:
    FileGuard(const char* n, const char* a);
    FileGuard(FILE* pp);
    ~FileGuard();
	
    operator FILE*();
	
private:
    FILE* p;
};

#endif
