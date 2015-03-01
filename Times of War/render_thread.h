#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

#include "..\DivCore\thread.h"

class render_thread : public thread
{
public:
	render_thread();
	~render_thread();

private:
	void run_thread();
};

#endif
