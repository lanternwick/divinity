Queue::Queue()
{
	first = NULL;
	end = NULL;
}

Queue::~Queue()
{
}


void Queue::push(DMSG item)
{
	QNode* c = first;
	QNode* d = first;
	while(c)
	{
		d = c;
		c = c->next;
	}
	c = new QNode;
	c->next = NULL;
	c->data = item;
	if(first) d->next = c;
	else 
	{
		first = c;
		end = c;
	}
}


DMSG Queue::pop()
{
	DMSG tmp = first->data;
	QNode* c;
	c = first;
	first = first->next;
	FreeMem(c);
	return tmp;
}

BOOL Queue::empty()
{
	return (first == NULL);
}

DMSG Queue::peek()
{
	return first->data;
}
