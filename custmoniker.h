#ifndef MONIKER_H
#define MONIKER_H

#include <string>

#pragma warning(disable : 4251)

class moniker
{
private:
	std::string name;
public:
	moniker()
	{
		name = "void_moniker";
	}
	moniker(const char* s)
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
	bool operator == (const moniker& rhs)
	{
		return rhs.name == name;
	}
};

inline std::ostream& operator <<(std::ostream& lhs, moniker& rhs)
{
	lhs << rhs();
	return lhs;
}

#endif