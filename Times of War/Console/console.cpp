#include "console.h"

using std::string;

console::console()
{

}

console::~console()
{
	functions.resize(0);

	obj_classes.resize(0);
	objects.resize(0);
}	

void console::add_function(string& function, console_functor* f_ptr, unsigned long access_level)
{
	func_def fd;
	parse_function(function, f_ptr, &fd);
	if(fd.function_name == "") return;
	string del = ":";
	string_tokenizer<> s(fd.function_name, del);
	if(s.size() == 1)
	{
		fd.access_level = access_level;
		functions.push_back(fd);
	}
	else
	{
		update_class_def(s, &fd);
	}
}

void console::parse_function(string& function, console_functor* f_ptr, func_def* fd)
{
	string del = "(); ,";
	string c_name;
	
	string_tokenizer<> s(function, del);
	if(s.token(0) != "function") return;
	
	fd->function_name = s.token(1);
	
	unsigned long i;
	for(i = 2; i < s.size(); ++i)
	{
		fd->function_args.push_back(s.token(i));
	}
	fd->function_ptr = f_ptr;
}

void console::add_object_class(string& _class)
{
	string del = "\t{} \n;:";
	class_def cd;
	func_def fd;
	unsigned long i;
	bool is_func = false;
	string_tokenizer<> s(_class, del);
	if(s.token(0) != "class") return;
	cd.class_name = s.token(1);
	for(i = 0; i < obj_classes.size(); ++i)
	{
		if(obj_classes[i].class_name == cd.class_name)
			return;
	}
	
	bool func_exists;
	for(i = 2; i < s.size(); ++i)
	{
		if(s.token(i) == "function")
		{
			parse_function("function " + s.token(i+1), NULL, &fd);
			func_exists = false;
			for(unsigned long j = 0; j < cd.class_functions.size(); ++j)
			{
				if(cd.class_functions[j].function_name == fd.function_name)
				{
					if(cd.class_functions[j].function_args == fd.function_args)
						func_exists = true;
				}
			}
			if(!func_exists)
				cd.class_functions.push_back(fd);
			++i;
			fd.clear();
			continue;
		}
		if(id_is_type(s.token(i)))
		{
			std::map<std::string, variant>::value_type j(s.token(i+1), variant(s.token(1), NULL, 0));
			cd.private_data.insert(j);
			i+=1;
			continue;
		}
	}
	obj_classes.push_back(cd);
}

void console::add_object_class_from_file(std::istream& in)
{
	std::string s;
	char c;
	while(!in.eof())
	{
		in.read(&c, 1);
		s += c;
	}
	add_object_class(s);
}

void console::update_class_def(string_tokenizer<>& s, func_def* fd)
{
	class_def* cd = NULL;
	unsigned long i;
	for(i = 0; i < obj_classes.size(); ++i)
	{
		if(obj_classes[i].class_name == s.token(0))
		{
			cd = &obj_classes[i];
			break;
		}
	}
	if(cd == NULL) return;
	for(i = 0; i < cd->class_functions.size(); ++i)
	{
		if(cd->class_functions[i].function_name == s.token(1))
		{
			if(cd->class_functions[i].function_args == fd->function_args)
			{
				cd->class_functions[i].function_ptr = fd->function_ptr;
				return;
			}
		}
	}	
}

bool console::id_is_type(string& id)
{
	if(id == "int") return true;
	if(id == "string") return true;
	if(id == "long") return true;
	if(id == "float") return true;
	return false;
}

unsigned long console::exec_command(string& command)
{
	string del = "() ,;";
	size_t size=-2;
	while((size=command.find("\"", size+2))!=command.npos)
		command.replace(size, 1, "\a\"");
	string_tokenizer<> s(command, del);
	del = ".";
	if(!s.size()) return -1;
	string_tokenizer<> n(s.token(0), del);
	if(n.size() == 1)
	{
		return exec_function(s);
	}
	else
	{
		return exec_object_function(n, s);
	}
}

bool console::arg_type_equivalent(string arg_t_1, string arg_t_2)
{
	if(arg_t_1 == arg_t_2) return true;
	if((arg_t_1 == "string") && (arg_t_2 != "string")) return false;
	if((arg_t_2 == "string") && (arg_t_1 != "string")) return false;
	
	return true;

}
unsigned long console::exec_function(string_tokenizer<>& s)
{
	string func = s.token(0);
	std::vector<variant> args;
	func_def* fd = NULL;
	variant v;
	unsigned long i;
	for(i = 0; i < functions.size(); ++i)
	{
		if(functions[i].function_name == func)
		{
			fd = &functions[i];
		
			if(!fd) return 1;
			if(fd->access_level > access_level) return 4;
			if((s.size() - 1) != fd->function_args.size()) continue; // more or less arguments provided
			unsigned long c = 0;
			bool args_ok = true;
			for(int j = 1; j < s.size(); ++j)
			{
				string_val_to_variant(fd->function_args[c], s.token(j), &v);
				args.push_back(v);
				args_ok &= arg_type_equivalent(args[c].type(), fd->function_args[c]);
			}
			if(!args_ok) continue;
			if(!fd->function_ptr) return 3;
			fd->function_ptr->f_call(this, NULL, NULL, args);
			break;
		}
	}
	return 0;
}

unsigned long console::exec_object_function(string_tokenizer<>& name, string_tokenizer<>& s)
// pre: name contains object name in token(0), and function name in token(1)
//      s contains <object name>.<function name> in token(0), and function args following
{
	string object = name.token(0);
	string func = name.token(1);
	std::vector<variant> args;
	func_def* fd = NULL;
	class_data* obj = NULL;
	class_def* cd = NULL;
	variant v;
	unsigned long i;
	for(i = 0; i < objects.size(); ++i)
	{
		if(objects[i].object_name == object)
		{
			obj = &objects[i];
			break;
		}
	}
	if(!obj) return 1;
	if(obj->access_level > access_level) return 7;
	for(i = 0; i < obj_classes.size(); ++i)
	{
		if(obj_classes[i].class_name == obj->class_name)
		{
			cd = &obj_classes[i];
			break;
		}
	}
	if(!cd) return 2;

	for(i = 0; i < cd->class_functions.size(); ++i)
	{
		if(cd->class_functions[i].function_name == func)
		{
			fd = &cd->class_functions[i];
	
		
			//if(!fd) return 3;
			if(fd)
			{
				if(fd->access_level > access_level) return 7;
				if((s.size() - 1) != fd->function_args.size()) continue; // more or less arguments provided
			}
			unsigned long c = 0;
			bool args_ok = true;
			for(long j = 1; j < s.size(); ++j)
			{
				string_val_to_variant(fd->function_args[c], s.token(j), &v);
				args.push_back(v);
				if(fd)
					args_ok &= arg_type_equivalent(args[c].type(), fd->function_args[c]);
			}
			if(fd)
				if(!args_ok) continue;
			if(obj->code_Object)
			{
				obj->code_Object->Dispatch(func, this, obj, cd, args);
				return 0;
			}
			if(!fd) return 6;
			if(!fd->function_ptr) return 6;
			fd->function_ptr->f_call(this, obj, cd, args);
			break;
		}
	}
	return 0;
}

void console::string_val_to_variant(string& type, string& value, variant* out)
{
	float f;
	long l;
	string s;
	if(value[0] == '\"') // string
	{
		out->insert((void*)(value.c_str()+1), value.size()-2);
		s = "string";
		out->set_type(s);
		return;
	}
	if(type == "float")
	{
		f = (float)atof(value.c_str());
		out->insert(&f, sizeof(float));
		s = "float";
		out->set_type(s);
	}
	if((type == "long") || (type == "int"))
	{
		l = atol(value.c_str());
		out->insert(&l, sizeof(long));
		s = "long";
		out->set_type(s);
	}
}

void console::create_object(string& class_name, string& object_name, unsigned long access_level)
{
	class_data* obj = NULL;
	class_data data;
	class_def* cd = NULL;
	variant v;
	unsigned long i;
	for(i = 0; i < objects.size(); ++i)
	{
		if(objects[i].object_name == object_name)
		{
			obj = &objects[i];
			break;
		}
	}
	if(obj) return;
	for(i = 0; i < obj_classes.size(); ++i)
	{
		if(obj_classes[i].class_name == class_name)
		{
			cd = &obj_classes[i];
			break;
		}
	}
	if(!cd) return;
	data.class_name = class_name;
	data.object_name = object_name;
	
	data.private_data = cd->private_data;
	std::vector<variant> con;
	for(i = 0; i < cd->class_functions.size(); ++i)
	{
		if(cd->class_functions[i].function_name == cd->class_name)
		{
			if(cd->class_functions[i].function_ptr)
				cd->class_functions[i].function_ptr->f_call(this, &data, cd, con);
			break;
		}
	}
	data.access_level = access_level;
	objects.push_back(data);
}

void console::create_object_ex(std::string& class_name, std::string& object_name, IConsoleBind* code_object, unsigned long access_level)
{
	class_data* obj = NULL;
	class_data data;
	class_def* cd = NULL;
	variant v;
	unsigned long i;
	for(i = 0; i < objects.size(); ++i)
	{
		if(objects[i].object_name == object_name)
		{
			obj = &objects[i];
			break;
		}
	}
	if(obj) return;
	for(i = 0; i < obj_classes.size(); ++i)
	{
		if(obj_classes[i].class_name == class_name)
		{
			cd = &obj_classes[i];
			break;
		}
	}
	if(!cd) return;
	data.class_name = class_name;
	data.object_name = object_name;
	
	data.private_data = cd->private_data;
	data.code_Object = code_object;
	std::vector<variant> con;
	for(i = 0; i < cd->class_functions.size(); ++i)
	{
		if(cd->class_functions[i].function_name == cd->class_name)
		{
			if(cd->class_functions[i].function_ptr)
				cd->class_functions[i].function_ptr->f_call(this, &data, cd, con);
			break;
		}
	}
	data.access_level = access_level;
	objects.push_back(data);
}

void console::delete_object(std::string& object_name)
{
	//class_data obj;
	unsigned long i;
	for(i = 0; i < objects.size(); ++i)
	{
		if(objects[i].object_name == object_name)
		{
			objects[i] = objects[objects.size()-1];
			objects.resize(objects.size()-1);
			return;
		}
	}
}

void console::set_security_level(unsigned long level)
{
	access_level = level;
}