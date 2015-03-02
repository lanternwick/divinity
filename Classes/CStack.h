#ifndef _CSTACK_H
#define _CSTACK_H

#include <vector>

template<class t>
class CStack 
{
public:
	CStack()
	{
		top = 0;
		myItems = 0;
	}

	~CStack() { }

	VOID push(t data)
	{
		if(top >= items.size())
			items.resize(top+10);
		items[top] = data;
		++top;
		++myItems;
		return;
	}

	t pop()
	{
		if(0 == top) return 0;
		t temp = items[top-1];
		--top;
		--myItems;
		return temp;
	}

	t peek()
	{
		return items[top-1];
	}

	VOID Clear()
	{
		top = 0;
	}

	long Items()
	{
		return myItems;
	}
protected:
	std::vector<t> items;
	INT top;
	long myItems;
};

#endif