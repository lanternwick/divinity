#ifndef UICONSOLEWND_H
#define UICONSOLEWND_H

#pragma warning(disable : 4786)

#include "UIBaseWindow.h"
#include "..\Common\define_exports.h"
#include "..\console\console.h"

// console window messages
// we reserve the first 256 messages for system messages
#define CONMSG_NOMSG			256
#define CONMSG_SETPROMPT		257
#define CONMSG_GETBUFFER		258
#define CONMSG_POSTKEY			259
#define CONMSG_EXPAND			260
#define CONMSG_CONTRACT			261
#define CONMSG_INCOFFSET		262
#define CONMSG_DECOFFSET		263
#define CONMSG_NEXTHIST			264
#define CONMSG_PREVHIST			265
#define CONMSG_SETTEXTCOLOR		266
#define CONMSG_SETFONT			267
#define CONMSG_CLEARBUFFER		268
#define CONMSG_SAVECMD			269
#define CONMSG_SETCONSOLE		270

class _API UIConsoleWnd : public UIBaseWindow
{
public:
	UIConsoleWnd();
	UIConsoleWnd(char* id);
	virtual ~UIConsoleWnd();

	virtual RESULT Render();

	virtual void send_message(window_message& msg);
	virtual bool Keyboard_KeyDown(char key);
	virtual bool Keyboard_KeyUp(char key);
private:
	std::string m_command_buffer;
	int m_state;
	unsigned long m_text_color;
	const float m_scale;
	std::string m_prompt;
	std::vector<std::string> m_cmd_hist;
	int m_cmd_hist_index;
	int m_line_offset;
	moniker m_font;
	console* m_console;
	bool m_opening;
};

#endif
