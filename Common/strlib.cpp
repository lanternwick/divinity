#include "strlib.h"
#include <string.h>

void mid(char** r, const char* src, long start, long len)
{
	if(len == 0) return;
	*r = new char[len+1];
	memcpy(*r, (void*)&(src[start]), len);
	(*r)[len] = 0;
	return;
}

long instr(long start, char* src, char* find)
{
	int i = 0;
	int pos = start;
	int len = strlen(find);
	char* p = &(src[start]);
	while(*p)
	{
		if(*p == find[i])
			++i;
		else
			i = 0;
		++pos;
		if(i == len)
			return pos-len+1;
		
		++p;
	}
	if(i == 0) return -1;
	return -1;
}

void replace(char** r, char* src, char* find, char* repl)
{
	int i = 0;
	char* tempBuff = NULL;
	long p = 0;
	while((p=instr(p+1, src, find)) != -1)
	{
		++i;
		++p;
	}
	long finallen = strlen(src);
	long findlen = strlen(find);
	long repllen = strlen(repl);
	finallen += repllen*i;
	finallen -= findlen*i;
	*r = new char[finallen+1];
	(*r)[0] = NULL;
	long lastPos = 0;
	long thisPos;
	for(int j = 0; j < i; ++j)
	{
		thisPos = instr(lastPos+1, src, find);
		mid(&tempBuff, src, lastPos, thisPos - lastPos - 1);
		strcat(*r, tempBuff);
		strcat(*r, repl);
		delete tempBuff;
		lastPos = thisPos+findlen-1;
	}
	if(strlen(*r) == (unsigned int)finallen)
		return;
	strcat(*r, &(src[lastPos]));
	return;
}

void triml(char** r, const char* src)
{
	long srclen = strlen(src);
	for(int i = 0; i < srclen; ++i)
	{
		if(src[i] != ' ')
		{
			break;
		}
	}
	*r = new char[srclen-i];
	strcpy(*r, &(src[i]));
	return;
}

void trimr(char** r, const char* src)
{
	long srclen = strlen(src);
	for(int i = srclen-1; i > 0; --i)
	{
		if(src[i] != ' ')
		{
			++i;
			break;
		}
	}
	*r = new char[srclen-(srclen-i)+1];
	memcpy(*r, src, srclen-(srclen-i));
	(*r)[srclen-(srclen-i)] = 0;

	return;
}

void trim(char** r, const char* src)
{
	trimr(r, src);
	triml(r, *r);
}