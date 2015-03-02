#ifndef CONSOLE_FUNCTOR_H
#define CONSOLE_FUNCTOR_H

class console;
struct class_data;
struct class_def;
#include <vector>
#include "variant.h"

class console_functor
{
public:
	virtual void f_call(console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)=0;
};

#endif