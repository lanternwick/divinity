#ifndef SVR_QUIT_FUNC_H
#define SVR_QUIT_FUNC_H

#include "console_functor.h"

class quit : public console_functor
{
public:
	quit(volatile bool* running);
	void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters);
private:
	volatile bool* m_ext_running;
};

#endif
