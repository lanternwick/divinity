#include "UICommandMap.h"

UICommandMap::UICommandMap()
{

}

UICommandMap::~UICommandMap()
{

}

void UICommandMap::Bind(char key, std::string& command)
{
	command_it i = m_commands.find(key);
	if(i == m_commands.end()) // key not yet bound, create new binding...
	{
		std::map<char, std::string>::value_type i(key, command);
		//i.first = key;
		//i.second = command;
		m_commands.insert(i);	
	}
	else
	{
		(*i).second = command;
	}
}

std::string UICommandMap::Command(char key)
{
	command_it i = m_commands.find(key);
	if(i == m_commands.end())
	{
		return std::string("fail");
	}
	return (*i).second;
}

UICommandMap::command_it UICommandMap::begin()
{
	return m_commands.begin();
}

UICommandMap::command_it UICommandMap::end()
{
	return m_commands.end();
}