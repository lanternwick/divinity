#ifndef _MAP_H
#define _MAP_H

#include <malloc.h>
#include <string.h>
#include <vector>

template<class t, class s>
class map
{
public:
	map()
	{
		Clear();
	}

	~map()
	{
		Clear();
	}

	
	void Clear();
	void insert(t& first, s& second);
	unsigned long resize(unsigned long newsize);

	map& operator =(const map& rhs)
	{
		Clear();
		numItems = rhs.numItems;
		numFilled = rhs.numFilled;
		items1 = rhs.items1;
		items2 = rhs.items2;
		return *this;
	}

	unsigned long size()
	{
		return numItems;
	}

	t& operator [](s index);
	s& operator [](t index);
private:
	custom::vector<t> items1;
	custom::vector<s> items2;
	unsigned long numItems;
	unsigned long numFilled;

};

template <class t, class s>
void map<t, s>::Clear()
{
	numFilled = numItems = 0;
	items1.resize(0);
	items2.resize(0);
}

template <class t, class s>
unsigned long map<t, s>::resize(unsigned long newsize)
{
	items1.resize(newsize);
	items2.resize(newsize);
	numItems = newsize;
	return numItems;
}
template <class t, class s>
void map<t, s>::insert(t& first, s& second)
{
	if(numFilled == numItems)
	{
		resize(numItems+1);
	}
	items1[numFilled] = first;
	
	items2[numFilled] = second;
	
	++numFilled;
}

template <class t, class s>
t& map<t, s>::operator [](s index)
{
	for(unsigned int i = 0; i < numItems; ++i)
	{
		if(items2[i] == index)
		{
			return items1[i];
		}
	}
	throw "not found";
}

template <class t, class s>
s& map<t, s>::operator [](t index)
{
	for(unsigned int i = 0; i < numItems; ++i)
	{
		if(items1[i] == index)
		{
			return items2[i];
		}
	}
	throw "not found";
}

/*template <class t, class s>
t map<t, s>::operator [](s index)
{
	return [](index);
}

template <class t, class s>
s map<t, s>::operator [](t index)
{
	return [](index);
}*/

#endif