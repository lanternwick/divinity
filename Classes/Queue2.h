#ifndef _QUEUE_H
#define _QUEUE_H

struct SYSTEM_API QNode
{
	DMSG data;
	QNode* next;
};


class SYSTEM_API Queue
{
public:
	Queue();
	~Queue();

	void push(DMSG item);
	DMSG pop();
	BOOL empty();
	DMSG peek();
private:
	QNode* first;
	QNode* end;
};


#endif