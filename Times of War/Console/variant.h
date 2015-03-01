#ifndef VARIANT_H
#define VARIANT_H

#pragma warning(disable:4786)

#include "BinaryData.h"
#include <string>
#include <vector>


using custom::binary_data;

class variant
{
public:
	variant();
	variant(std::string& type, void* data, unsigned long size);
	variant(const variant& v);
	~variant();

	std::string type();
	void set_type(std::string& s);
	unsigned long size();
	void insert(void* data, unsigned long size);
	void append(void* data, unsigned long size);

	void extract(void* data);
	std::string as_if_string();
	float as_if_float();
	long as_if_long();
	int as_if_int();
	std::vector<std::string> as_if_string_array();
	variant& operator = (std::string& rhs);
	variant& operator += (std::string& rhs);
	variant& operator = (long rhs);
	variant& operator = (float rhs);
	variant& operator = (const variant& rhs);
private:
	binary_data m_data;
	std::string m_type;
};

#endif
