#ifndef ITIMER_H
#define ITIMER_H

class ITimer
{
public:
	virtual void On_Timer(float t)=0;
};

#endif