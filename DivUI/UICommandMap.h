#ifndef UICOMMANDMAP_H
#define UICOMMANDMAP_H

#pragma warning(disable:4786)

#include <map>
#include <string>
#include "..\Common\define_exports.h"

class _API UICommandMap
{
public:
	typedef std::map<char, std::string>::iterator command_it;
	
	UICommandMap();
	~UICommandMap();

	void Bind(char key, std::string& command);
	std::string Command(char key);
	char StringToKeyConst(std::string& keyname);
	command_it begin();
	command_it end();
private:
	std::map<char, std::string> m_commands;
};

#endif
