#ifndef _QUEUE_H
#define _QUEUE_H

#include <vector>

template <class t>
class Queue
{
public:
	Queue();
	~Queue();

	void push(t item);
	t pop();
	BOOL empty();
	t peek();
private:
	UINT front;
	UINT end;
	std::vector<t> items;
};

template <class t>
Queue<t>::Queue()
{
	items.resize(0);
	front = 0;
	end = 0;
}

template <class t>
Queue<t>::~Queue()
{

}

template <class t>
void Queue<t>::push(t item)
{
	items.push_back(item);
	++end;
}

template <class t>
t Queue<t>::pop()
{
	t tmp = items[front];
	++front;
	if(front == end)
	{
		front = 0;
		end = 0;
		items.resize(0);
	}
	return tmp;
}

template <class t>
BOOL Queue<t>::empty()
{
	return (front == end);
}

template <class t>
t Queue<t>::peek()
{
	return items[front];
}

#endif