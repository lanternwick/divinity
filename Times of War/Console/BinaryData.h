#ifndef _BINARY_DATA_H
#define _BINARY_DATA_H

#include <memory.h>


namespace custom
{
	class binary_data
	{
	public:
		binary_data();
		~binary_data();

		void* extract(void* out, unsigned long size, unsigned long offset=0);
		void  insert(void* in, unsigned long size, unsigned long offset=0);
		unsigned long resize(unsigned long newSize);
		unsigned long size();
	
		bool operator == (binary_data& rhs);
		bool operator != (binary_data& rhs);
		bool operator > (binary_data& rhs);
		bool operator < (binary_data& rhs);
		bool operator >= (binary_data& rhs);
		bool operator <= (binary_data& rhs);
		binary_data& operator = (const binary_data& rhs);

		binary_data& operator +=(binary_data& rhs);

		operator const void* () const;
		operator void* ();

	private:
		char* data;
		unsigned long m_size;
	};
};

#endif