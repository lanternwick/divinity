#include "Fileguard.h"

FileGuard::FileGuard(const char* n, const char* a)
{
	p = fopen(n, a); 
}

FileGuard::FileGuard(FILE* pp)
{
	p = pp;
}

FileGuard::~FileGuard()
{
	if(p)
		fclose(p);
}
	
FileGuard::operator FILE*()
{
	return p;
}