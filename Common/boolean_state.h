#ifndef BOOLEAN_STATE_H
#define BOOLEAN_STATE_H

class boolean_state
{
public:
	boolean_state(){}
	virtual ~boolean_state(){}

	virtual void save_state()=0;
	virtual void restore_state()=0;
};

#endif