#include "UIConsoleWnd.h"
#include "UserInterface.h"
#include "KeyConstants.h"

UIConsoleWnd::UIConsoleWnd()
:m_scale(1.0f)
{
	m_prompt = "> ";
	m_text_color = 0xff000000;
	m_state = 0;
	m_font = NULL;
	m_line_offset = 0;
}

UIConsoleWnd::UIConsoleWnd(char* id)
:m_scale(1.0f)
{
	m_window_name() = id;
	m_prompt = "> ";
	m_text_color = 0xff000000;
	m_state = 0;
	m_font = NULL;
	m_line_offset = 0;

}

UIConsoleWnd::~UIConsoleWnd()
{
	if(m_window)
	{
		delete m_window;
		m_window = NULL;
	}

}

RESULT UIConsoleWnd::Render()
{

	if(!m_font)
		return OK;
	if(!m_log)
		return OK;
	SIZE s = m_font->GetCellSize();
	RECT tr = {m_pos.x+5, m_pos.y+5, m_pos.x + m_size.cx, (m_pos.y-5) + (m_size.cy-5) - s.cy*m_scale};
	std::string log_lines;
	if(m_state)
	{
		UIBaseWindow::Render();
		/*if(!m_ui)
		{
			m_mutex.unlock();
			return OK;
		}*/

		if(m_state == 1)
		{
			m_log->tail(log_lines, 1, m_line_offset);
			m_font->DrawText(log_lines.c_str(), tr, NULL, m_scale, m_text_color);
			tr.top += s.cy*m_scale*1;
			/*m_ui->DrawText(m_font, m_prompt.c_str(), tr, NULL, m_scale, m_textColor);
			tr.left += s.cx*m_scale*m_prompt.size();
			if(m_input_buffer.length())
				m_ui->DrawText(m_font, m_input_buffer.c_str(), tr, NULL, m_scale, m_textColor);*/
		}
		if(m_state == 2)
		{
			m_log->tail(log_lines, 3, m_line_offset);
			m_font->DrawText(log_lines.c_str(), tr, NULL, m_scale, m_text_color);
			tr.top += s.cy*m_scale*3;
			tr.bottom += s.cy*m_scale;
			m_font->DrawText(m_prompt.c_str(), tr, NULL, m_scale, m_text_color);
			tr.left += s.cx*m_scale*m_prompt.size();
			if(m_command_buffer.length())
				m_font->DrawText(m_command_buffer.c_str(), tr, NULL, m_scale, m_text_color);
		}
		if(m_state == 3)
		{

			m_log->tail(log_lines, 20, m_line_offset);
			m_font->DrawText(log_lines.c_str(), tr, NULL, m_scale, m_text_color);
			
			tr.top += s.cy*m_scale*19;
			tr.bottom += s.cy*m_scale+10;
			m_font->DrawText(m_prompt.c_str(), tr, NULL, m_scale, m_text_color);
			tr.left += s.cx*m_scale*m_prompt.size();
			if(m_command_buffer.length())
				m_font->DrawText(m_command_buffer.c_str(), tr, NULL, m_scale, m_text_color);
		}
		//re.left = 
		
	}
	
	return OK;
}

void UIConsoleWnd::send_message(window_message& msg)
{
	SIZE s;
	switch(msg.message)
	{
	case CONMSG_NOMSG:

		break;
	case CONMSG_SETPROMPT:
		m_prompt = msg.string_in;
		break;
	case CONMSG_GETBUFFER:
		msg.string_ret = m_command_buffer;
		break;
	case CONMSG_POSTKEY:
		if(char(msg.ulong_in[0]) == '\b')
		{
			if(m_command_buffer.size())
			{
				m_command_buffer.resize(m_command_buffer.size()-1);
			}
			break;
		}
		if((char(msg.ulong_in[0]) == '`') || (char(msg.ulong_in[0]) == '~'))
		{
			break;
		}
		m_command_buffer += (char)msg.ulong_in[0];
		break;
	case CONMSG_EXPAND:
		m_state = 3;
		s = m_font->GetCellSize();
		this->m_size.cy = s.cy*m_scale*20 + 5;
		Update();
		m_opening = true;
		break;
	case CONMSG_CONTRACT:
		m_state = 0;
		this->m_size.cy = 0;
		Update();
		break;
	case CONMSG_INCOFFSET:
		m_line_offset += 4;
		break;
	case CONMSG_DECOFFSET:
		m_line_offset -= 4;
		break; 
	case CONMSG_NEXTHIST:
		if(++m_cmd_hist_index>m_cmd_hist.size()-1)
		{
			m_cmd_hist_index = 0;
		}
		m_command_buffer = m_cmd_hist[m_cmd_hist_index];
		break;
	case CONMSG_PREVHIST:
		if(--m_cmd_hist_index<0)
		{
			m_cmd_hist_index = m_cmd_hist.size()-1;
		}
		m_command_buffer = m_cmd_hist[m_cmd_hist_index];
		break;
	case CONMSG_SETTEXTCOLOR:
		m_text_color = msg.ulong_in[0];
		break;
	case CONMSG_SETFONT:
		m_font = (Font*)msg.ulong_in[0];
	case CONMSG_CLEARBUFFER:
		m_command_buffer.erase();
		break;
	case CONMSG_SAVECMD:
		if(m_cmd_hist.size() == 0)
		{
			m_cmd_hist.push_back(m_command_buffer);
		}
		else
		{
			m_cmd_hist.back() = m_command_buffer;
		}
		m_cmd_hist.push_back(std::string(""));
		m_cmd_hist_index = m_cmd_hist.size()-1;
		break;
	case CONMSG_SETCONSOLE:
		m_console = (console*)msg.ulong_in[0];
	default:
		break;
	}
}

bool UIConsoleWnd::Keyboard_KeyDown(IInputDevice* input, char key)
{
	window_message m;
	std::string s;
	bool shift = (input->GetKey(DIV_KEY_RSHIFT) | input->GetKey(DIV_KEY_LSHIFT));
	m_opening = false;
	switch((unsigned char)key)
	{
	case DIV_KEY_RETURN:
		s = m_command_buffer;
		m_log->write(s.c_str(), 0);
		m_log->write("\n", 0);
		//try
		{
			m.message = CONMSG_SAVECMD;
			send_message(m);
			m.message = CONMSG_CLEARBUFFER;
			send_message(m);
			if(s[0] == '/' && (s[1] == 'l' || s[1] == 'L'))
			{
				//lua_lua_dobuffer(s.c_str()+3, s.size()-3);
			}
			else
			{
				if(m_console)
				{
					m_console->exec_command(s);
				}
				else
				{
					m_log->write("Console not correctly initialized!\n", 1);
				}
			}
		}
		//catch(...)
		{
		//	Core::WriteLog("Exception occured while executing console command\n", 1);
		}
		break;

	case DIV_KEY_PGUP:
		// scroll log up
		m.message = CONMSG_INCOFFSET;
		send_message(m);
		//uw->SetOffset(uw->GetOffset()+uw->GetNumLinesAtState(uw->GetState()));
		break;
	case DIV_KEY_PGDN:
		//scroll log down
		m.message = CONMSG_DECOFFSET;
		send_message(m);
		//uw->SetOffset(uw->GetOffset()-uw->GetNumLinesAtState(uw->GetState()));
		break;
	case DIV_KEY_UP:
		//next hist command
		m.message = CONMSG_NEXTHIST;
		send_message(m);
		//uw->NextCmd();
		break;
	case DIV_KEY_DOWN:
		// prev hist command
		m.message = CONMSG_PREVHIST;
		send_message(m);
		//uw->PrevCmd();
		break;
	case DIV_KEY_RSHIFT:
	case DIV_KEY_LSHIFT:
		return true;
	default:
		// post the key to the buffer
		m.message = CONMSG_POSTKEY;
		m.ulong_in[0] = input->KeyToChar(key, shift);
		if(m.ulong_in[0])
		{
			send_message(m);
		}
		//uw->AddKeyToBuffer(singleton<UserInterface*>::instance()->KeyToChar(key, shift));
		break;
		
	}
	return false;
}

bool UIConsoleWnd::Keyboard_KeyUp(IInputDevice* input, char key)
{
	window_message m;
	switch(key)
	{
	case DIV_KEY_GRAVE:
		if(m_opening)
		{
			break;
		}
		m.message = CONMSG_CONTRACT;
		send_message(m);
		//singleton<UserInterface*>::instance()->Windows().ActiveWindow(NULL);
		return false;
		break;
	default:
		return true;
		break;
	};
	return true;
}

int UIConsoleWnd::set_font(Font* fnt)
{
	m_font = fnt;
	return 0;
}