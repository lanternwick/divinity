#ifndef _SYSTIMER_H
#define _SYSTIMER_H


class SYSTimer
{
public:
	SYSTimer();

	VOID Interval(DWORD NewInterval);
	DWORD Interval();
	BOOL Query();
	DWORD Time();
	DWORD Elapsed(UINT Index);
	UINT Start();
	DWORD GetStartTime(UINT Index);
protected:
	DWORD myInterval;
	LARGE_INTEGER startTime;
	DWORD myTimes[1024];
	DWORD NumTimes;
	LARGE_INTEGER GameTime;
	BOOL Paused;
};

SYSTimer::SYSTimer()
{
	startTime = GetTickCount();
	NumTimes = 0;
	GameTime = 0;
	Paused = FALSE;
}

VOID SYSTimer::Interval(DWORD NewInterval)
{
	myInterval = NewInterval;
	return;
}

DWORD SYSTimer::Interval()
{
	return myInterval;
}

BOOL SYSTimer::Query()
{
	static LARGE_INTEGER Frequency = 0;
	static LARGE_INTEGER Last = 0;
	LARGE_INTEGER GameTime = 0;
	if(Frequency == 0)
	{
		QueryPerformanceFrequency(&Frequency);
	}
	if(Paused)
	{
		return FALSE;
	}
	LARGE_INTEGER t, t2;
	QueryPerformanceCounter(&t);
	t /= Frequency;
	t2 = t - Last;
	GameTime += t2;
	if(startTime + interval <= t)
	{
		last = t;
		startTime = t;
		return TRUE;
	}
	return FALSE;
}


}

DWORD SYSTimer::Time()
{
	return GetTickCount();
}

DWORD SYSTimer::Elapsed(UINT Index)
{
	if(1024 <= Index) return -1;
	return GetTickCount() - myTimes[Index];
}

UINT SYSTimer::Start()
{
	//myTimes.resize(myTimes.length());
	myTimes[NumTimes] = GetTickCount();
	NumTimes++;
	return NumTimes - 1;
}

DWORD SYSTimer::GetStartTime(UINT Index)
{
	return myTimes[Index];
}

#endif