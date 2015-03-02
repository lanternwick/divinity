#include "variant.h"

using std::string;

variant::variant()
{

}

variant::variant(string& type, void* data, unsigned long size)
{
	insert(data, size);
	m_type = type;
}

variant::variant(const variant& v)
{
	m_data = v.m_data;
	m_type = v.m_type;
}
variant::~variant()
{
	m_data.resize(0);
}

string variant::type()
{
	return string(m_type);
}

void variant::set_type(string& s)
{
	m_type = s;
}

unsigned long variant::size()
{
	return m_data.size();
}

void variant::insert(void* data, unsigned long size)
{
	m_data.resize(size);
	m_data.insert(data, size);
}

void variant::append(void* data, unsigned long size)
{
	unsigned long osize = m_data.size();
	m_data.resize(osize+size+3);
	short x = 0;
	m_data.insert(&x, 1, osize);
	m_data.insert(data, size, osize+1);
	m_data.insert(&x, 2, osize+size+1);
	m_type = "array";
}

void variant::extract(void* data)
{
	m_data.extract(data, m_data.size());
}

string variant::as_if_string()
{
	if(m_data.size() == 0)
	{
		return std::string("");
	}
	char* c = new char[m_data.size()+1];
	extract(c);
	c[m_data.size()] = 0;
	string r = c;
	delete c;
	return r;
}

float variant::as_if_float()
{
	float f;
	if(m_data.size() == 0)
		return 0.0f;
	m_data.extract(&f, sizeof(float));
	return f;
}

long variant::as_if_long()
{
	long l;
	if(m_data.size() == 0)
		return 0;
	m_data.extract(&l, sizeof(long));
	return l;
}

int variant::as_if_int()
{
	int i;
	if(m_data.size() == 0)
		return 0;
	m_data.extract(&i, sizeof(int));
	return i;
}

std::vector<string> variant::as_if_string_array()
{
	std::vector<string> v;
	if(m_data.size() == 0)
	{
		return v;
	}
	
	char* c = new char[m_data.size()+1];
	extract(c);
	//c[m_data.size()] = 0;
	string r;
	char* c_p = c;
	for(;(*c_p) != 0 || (*(c_p+1))!= 0; ++c_p)
	{
		r += (*c_p);
		if(!(*c_p))
		{
			v.push_back(r);
			r = "";
		}
	}
	delete c;
	r += '\0';
	v.push_back(r);
	return v;
}

variant& variant::operator = (std::string& rhs)
{
	insert((void*)rhs.c_str(), rhs.size());
	m_type = std::string("string");
	return *this;
}

variant& variant::operator += (std::string& rhs)
{
	append((void*)rhs.c_str(), rhs.size());
	return *this;
}

variant& variant::operator = (long rhs)
{
	insert(&rhs, sizeof(rhs));
	m_type = std::string("long");
	return *this;
}

variant& variant::operator = (float rhs)
{
	insert(&rhs, sizeof(rhs));
	m_type = std::string("float");
	return *this;
}

variant& variant::operator =(const variant& rhs)
{
	m_data = rhs.m_data;
	m_type = rhs.m_type;
	return *this;
}