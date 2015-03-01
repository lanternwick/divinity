#ifndef STRING_TOKENIZER_H
#define STRING_TOKENIZER_H

#pragma warning(disable : 4786)

#include <vector>
#include <string>

template<class str_type=std::string>
class string_tokenizer
{
public:
	string_tokenizer(str_type& input, str_type& delimeters)
	{
		tokenize(input, delimeters);
	}

	~string_tokenizer()
	{
	}

	str_type& token(unsigned long index)
	{
		return tokens[index];
	}
	unsigned long size()
	{
		return tokens.size();
	}

private:
	std::vector<str_type> tokens;
	void tokenize(str_type& input, str_type& delimeters)
	{
		unsigned long i, j, k;
		str_type temp;
		bool del_found = false;
		bool is_protected_chunk = false;
		k = 0;
		for(i = 0; i < input.size(); ++i)
		{
			del_found = false;
			if(input[i] == '\a')
			{
				is_protected_chunk = !is_protected_chunk;
				continue;
			}
			if(!is_protected_chunk)
			{
				for(j = 0; j < delimeters.size(); ++j)
				{
					if(input[i] == delimeters[j])
					{
						del_found = true;
					}
				}
			}
			if(del_found)
			{
				if(temp.size()) tokens.push_back(temp);
				temp = "";
			}
			else
			{
				temp += input[i];
			}
		}
		if(temp.size()) tokens.push_back(temp);
	}
};

#endif
