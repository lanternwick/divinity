#ifndef _FILEGUARD_H
#define _FILEGUARD_H

class FileGuard
{
public:
    FileGuard(const char* n, const char* a) {p = fopen(n, a); }
    FileGuard(FILE* pp) { p = pp; }
    ~FileGuard() { fclose(p); }
	
    operator FILE*() { return p; }
	
private:
    FILE* p;
};

#endif
