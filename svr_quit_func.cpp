#include "svr_quit_func.h"

quit::quit(volatile bool* running)
{
	m_ext_running = running;
}

void quit::f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)
{
	(*m_ext_running) = false;
}