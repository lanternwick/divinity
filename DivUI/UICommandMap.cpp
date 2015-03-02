#include "UICommandMap.h"
#include "KeyConstants.h"

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

char UICommandMap::StringToKeyConst(std::string& keyname)
{
#define CONVERT(x, y) if(keyname == x) return (char)y;
	CONVERT("esc", DIV_KEY_ESCAPE);
	CONVERT("1", DIV_KEY_1);
	CONVERT("2", DIV_KEY_2);
	CONVERT("3", DIV_KEY_3);
	CONVERT("4", DIV_KEY_4);
	CONVERT("5", DIV_KEY_5);
	CONVERT("6", DIV_KEY_6);
	CONVERT("7", DIV_KEY_7);
	CONVERT("8", DIV_KEY_8);
	CONVERT("9", DIV_KEY_9);
	CONVERT("0", DIV_KEY_0);
	CONVERT("-", DIV_KEY_MINUS);
	CONVERT("=", DIV_KEY_EQUALS);
	CONVERT("backspace", DIV_KEY_BACK);
	CONVERT("tab", DIV_KEY_TAB);
	CONVERT("q", DIV_KEY_Q);
	CONVERT("w", DIV_KEY_W);
	CONVERT("e", DIV_KEY_E);
	CONVERT("r", DIV_KEY_R);
	CONVERT("t", DIV_KEY_T);
	CONVERT("y", DIV_KEY_Y);
	CONVERT("u", DIV_KEY_U);
	CONVERT("i", DIV_KEY_I);
	CONVERT("o", DIV_KEY_O);
	CONVERT("p", DIV_KEY_P);
	CONVERT("[", DIV_KEY_LBRACKET);
	CONVERT("]", DIV_KEY_RBRACKET);
	CONVERT("return", DIV_KEY_RETURN);
	CONVERT("l_ctrl", DIV_KEY_LCONTROL);
	CONVERT("a", DIV_KEY_A);
	CONVERT("s", DIV_KEY_S);
	CONVERT("d", DIV_KEY_D);
	CONVERT("f", DIV_KEY_F);
	CONVERT("g", DIV_KEY_G);
	CONVERT("h", DIV_KEY_H);
	CONVERT("j", DIV_KEY_J);
	CONVERT("k", DIV_KEY_K);
	CONVERT("l", DIV_KEY_L);
	CONVERT(";", DIV_KEY_SEMICOLON);
	CONVERT("'", DIV_KEY_APOSTROPHE);
	CONVERT("`", DIV_KEY_GRAVE);
	CONVERT("l_shift", DIV_KEY_LSHIFT);
	CONVERT("\\", DIV_KEY_BACKSLASH);
	CONVERT("z", DIV_KEY_Z);
	CONVERT("x", DIV_KEY_X);
	CONVERT("c", DIV_KEY_C);
	CONVERT("v", DIV_KEY_V);
	CONVERT("b", DIV_KEY_B);
	CONVERT("n", DIV_KEY_N);
	CONVERT("m", DIV_KEY_M);
	CONVERT(",", DIV_KEY_COMMA);
	CONVERT(".", DIV_KEY_PERIOD);
	CONVERT("/", DIV_KEY_SLASH);
	CONVERT("r_shift", DIV_KEY_RSHIFT);
	CONVERT("num_*", DIV_KEY_MULTIPLY);
	CONVERT("l_alt", DIV_KEY_LMENU);
	CONVERT("space", DIV_KEY_SPACE);
	CONVERT("f1", DIV_KEY_F1);
	CONVERT("f2", DIV_KEY_F2);
	CONVERT("f3", DIV_KEY_F3);
	CONVERT("f4", DIV_KEY_F4);
	CONVERT("f5", DIV_KEY_F5);
	CONVERT("f6", DIV_KEY_F6);
	CONVERT("f7", DIV_KEY_F7);
	CONVERT("f8", DIV_KEY_F8);
	CONVERT("f9", DIV_KEY_F9);
	CONVERT("f10", DIV_KEY_F10);
	CONVERT("num_7", DIV_KEY_NUMPAD7);
	CONVERT("num_8", DIV_KEY_NUMPAD8);
	CONVERT("num_9", DIV_KEY_NUMPAD9);
	CONVERT("num_-", DIV_KEY_SUBTRACT);
	CONVERT("num_4", DIV_KEY_NUMPAD4);
	CONVERT("num_5", DIV_KEY_NUMPAD5);
	CONVERT("num_6", DIV_KEY_NUMPAD6);
	CONVERT("num_+", DIV_KEY_ADD);
	CONVERT("num_1", DIV_KEY_NUMPAD1);
	CONVERT("num_2", DIV_KEY_NUMPAD2);
	CONVERT("num_3", DIV_KEY_NUMPAD3);
	CONVERT("num_0", DIV_KEY_NUMPAD0);
	CONVERT("num_.", DIV_KEY_DECIMAL);
	CONVERT("f11", DIV_KEY_F11);
	CONVERT("f12", DIV_KEY_F12);
	CONVERT("f13", DIV_KEY_F13);
	CONVERT("f14", DIV_KEY_F14);
	CONVERT("f15", DIV_KEY_F15);
	CONVERT("num_enter", DIV_KEY_NUMPADENTER);
	CONVERT("r_ctrl", DIV_KEY_RCONTROL);
	CONVERT("num_/", DIV_KEY_DIVIDE);
	CONVERT("r_alt", DIV_KEY_RMENU);
	CONVERT("home", DIV_KEY_HOME);
	CONVERT("up", DIV_KEY_UP);
	CONVERT("pgup", DIV_KEY_PRIOR);
	CONVERT("left", DIV_KEY_LEFT);
	CONVERT("right", DIV_KEY_RIGHT);
	CONVERT("end", DIV_KEY_END);
	CONVERT("down", DIV_KEY_DOWN);
	CONVERT("pgdn", DIV_KEY_NEXT);
	CONVERT("insert", DIV_KEY_INSERT);
	CONVERT("delete", DIV_KEY_DELETE);
	CONVERT("mouse_1_dn", DIV_KEY_MOUSE1_DOWN);
	CONVERT("mouse_1_up", DIV_KEY_MOUSE1_UP);
	CONVERT("mouse_2_dn", DIV_KEY_MOUSE2_DOWN);
	CONVERT("mouse_2_up", DIV_KEY_MOUSE2_UP);
	CONVERT("mouse_3_dn", DIV_KEY_MOUSE3_DOWN);
	CONVERT("mouse_3_up", DIV_KEY_MOUSE3_UP);
	CONVERT("mouse_4_dn", DIV_KEY_MOUSE4_DOWN);
	CONVERT("mouse_4_up", DIV_KEY_MOUSE4_UP);
	CONVERT("mouse_scroll_dn", DIV_KEY_MOUSESCROLL_DOWN);
	CONVERT("mouse_scroll_up", DIV_KEY_MOUSESCROLL_UP);
	
	
	
	return 0;
#undef CONVERT
}