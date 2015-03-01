#ifndef ICONSOLEBIND_H
#define ICONSOLEBIND_H

#include <vector>
#include <string>

class console;
struct class_data;
struct class_def;
#include "variant.h"

class IConsoleBind
{
public:
	virtual void Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, std::vector<variant>& parameters)=0;


};

#endif