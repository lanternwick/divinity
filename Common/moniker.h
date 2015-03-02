#ifndef MONIKER_H
#define MONIKER_H

#include <string>

class moniker
{
private:
	std::string name;
public:
	moniker()
	{
		name.resize(0);
	}
	moniker(char* s)
	{
		name = s;
	}
	std::string& operator()()
	{
		return name;
	}
};

#endif