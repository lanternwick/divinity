#ifndef CONSOLE_H
#define CONSOLE_H

#pragma warning (disable : 4786)

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "variant.h"
#include "string_tokenizer.h"
#include "console_functor.h"
#include "IConsoleBind.h"

struct class_data
{
	std::string class_name;
	std::string object_name;
	std::map<std::string, variant> private_data;
	class_data(){}
	~class_data()
	{
		private_data.clear();
	}
	IConsoleBind* code_Object;
	unsigned long access_level;
	class_data& operator=(const class_data& rhs)
	{
		class_name = rhs.class_name;
		object_name = rhs.object_name;
		private_data = rhs.private_data;
		code_Object = rhs.code_Object;
		access_level = rhs.access_level;
		return *this;
	}
	class_data(const class_data& cd)
	{
		*this = cd;
	}
};

struct class_def;
class console;

//typedef void con_func(console* c, class_data* _this, const class_def* def, custom::vector<variant>& parameters);

struct func_def
{
	std::string function_name;
	std::string function_descriptor;
	std::vector<std::string> function_args;
	console_functor* function_ptr;
	unsigned long access_level;
	~func_def()
	{
		function_args.resize(0);
	}
	void clear()
	{
		function_name.erase();
		function_args.resize(0);
		function_ptr = NULL;
		access_level = 0;
	}
};

struct alias_def
{
	std::string alias_name;
	std::string exec_function;
};

struct class_def
{
	std::string class_name;
	std::map<std::string, variant> private_data;
	std::vector<func_def> class_functions;
	class_def(){}
	~class_def()
	{
		private_data.clear();
		class_functions.resize(0);
	}
	class_def& operator=(const class_def& rhs)
	{
		class_name = rhs.class_name;
		private_data = rhs.private_data;
		class_functions = rhs.class_functions;
		return *this;
	}
	class_def(const class_def& cd)
	{
		*this = cd;
	}
};

class console
{
public:
	console();
	~console();

	void add_object_class(std::string& _class);
	void add_object_class_from_file(std::istream& in);
	void add_function(std::string& function, console_functor* f_ptr, unsigned long access_level=0);
	unsigned long exec_command(std::string& command);
	void create_object(std::string& class_name, std::string& object_name, unsigned long access_level=0);
	void create_object_ex(std::string& class_name, std::string& object_name, IConsoleBind* code_object, unsigned long access_level=0);
	void delete_object(std::string& object_name);
	void set_security_level(unsigned long level=0);

	void alias(std::string& alias, std::string& function_string);
	void command_list(std::vector<std::string>& cmd_list);
private:
	void update_class_def(string_tokenizer<>& s, func_def* fd);
	void parse_function(std::string& function, console_functor* f_ptr, func_def* fd);
	bool id_is_type(std::string& id);
	unsigned long exec_function(string_tokenizer<>& s);
	unsigned long exec_object_function(string_tokenizer<>& name, string_tokenizer<>& s);
	void string_val_to_variant(std::string& type, std::string& value, variant* out);
	bool arg_type_equivalent(std::string arg_t_1, std::string arg_t_2);
	std::vector<class_def> obj_classes;
	std::vector<class_data> objects;
	std::vector<func_def> functions;
	std::vector<alias_def> aliases;
	unsigned long access_level;
};

#endif
