#ifndef MONIKER_H
#define MONIKER_H

#include "define_exports.h"
#include <string>

#pragma warning(disable : 4251)

class _API moniker
{
private:
	std::string name;
public:
	moniker()
	{
		name = "void_moniker";
	}
	moniker(char* s)
	{
		name = s;
	}
	std::string operator()() const
	{
		return name;
	}
	std::string& operator()()
	{
		return name;
	}
	bool operator == (moniker& rhs)
	{
		return rhs.name == name;
	}
};

#endif