#include "UIConsoleWnd.h"

UIConsoleWnd::UIConsoleWnd()
: m_scale(1.0f)
{
	Core::AddTimer(this);
	m_ui = NULL;
	
}

UIConsoleWnd::UIConsoleWnd(char* id)
: m_scale(1.0f)
{
	Core::AddTimer(this);
	m_ui = NULL;
	m_id() = id;
}

UIConsoleWnd::~UIConsoleWnd()
{
	Core::RemoveTimer(this);
}

RESULT UIConsoleWnd::Render()
{
	RECT tr = {m_pos.x+5, m_pos.y+m_titlebar_overlap, m_pos.x + m_size.cx, m_pos.y + m_size.cy-5};
	float f = 1.0f/Core::GetRawTimeDelta();
	char buff[128];
	sprintf(buff, "FPS: %f", f);
	RECT re = { 0, m_device->height() * 0.90f, m_device->width(),m_device->height() };
	SIZE s = m_fontMan->GetFontCellSize("default");
	m_ui->DrawText(m_font, buff, re, NULL, 1.0f, 0xffffffff);
	if(m_state)
	{
		UIWindow::Render();
		if(!m_ui)
			return OK;

		if(m_state == 1)
		{
			m_ui->DrawText("default", Core::ReadLog(1), tr, NULL, m_scale, m_textColor);
			tr.top += s.cy*m_scale*1+m_titlebar_overlap;
			/*m_ui->DrawText(m_font, m_prompt.c_str(), tr, NULL, m_scale, m_textColor);
			tr.left += s.cx*m_scale*m_prompt.size();
			if(m_input_buffer.length())
				m_ui->DrawText(m_font, m_input_buffer.c_str(), tr, NULL, m_scale, m_textColor);*/
		}
		if(m_state == 2)
		{
			m_ui->DrawText("default", Core::ReadLog(3), tr, NULL, m_scale, m_textColor);
			tr.top += s.cy*m_scale*3;
			m_ui->DrawText(m_font, m_prompt.c_str(), tr, NULL, m_scale, m_textColor);
			tr.left += s.cx*m_scale*m_prompt.size();
			if(m_input_buffer.length())
				m_ui->DrawText(m_font, m_input_buffer.c_str(), tr, NULL, m_scale, m_textColor);
		}
		if(m_state == 3)
		{

			m_ui->DrawText("default", Core::ReadLog(19), tr, NULL, m_scale, m_textColor);
			
			tr.top += s.cy*m_scale*19;
			m_ui->DrawText(m_font, m_prompt.c_str(), tr, NULL, m_scale, m_textColor);
			tr.left += s.cx*m_scale*m_prompt.size();
			if(m_input_buffer.length())
				m_ui->DrawText(m_font, m_input_buffer.c_str(), tr, NULL, m_scale, m_textColor);
		}
		//re.left = 
		
	}
	return OK;
}

void UIConsoleWnd::SetUIPtr(UserInterface* ui)
{
	m_ui = ui;
}

void UIConsoleWnd::SetTextColor(unsigned long color)
{
	m_textColor = color;
}

void UIConsoleWnd::SetState(int state)
{
	m_state = state;
	SIZE s = m_fontMan->GetFontCellSize("default");
	if(state == 0)
	{
		this->m_size.cy = 0;
	}
	if(state == 1)
	{
		this->m_size.cy = s.cy*m_scale + 5+m_titlebar_overlap;
	}
	if(state == 2)
	{
		this->m_size.cy = s.cy*m_scale*4 + 5+m_titlebar_overlap;
	}
	if(state == 3)
	{
		this->m_size.cy = s.cy*m_scale*20 + 5+m_titlebar_overlap;
	}
	m_SyncVerts();
}

int UIConsoleWnd::GetState()
{
	return m_state;
}

void UIConsoleWnd::On_Timer(float t)
{
	
}

RESULT UIConsoleWnd::Think()
{
	return OK;
}

void UIConsoleWnd::SetPrompt(std::string& p)
{
	m_prompt = p;
}

void UIConsoleWnd::AddKeyToBuffer(char c)
{
	if((c == '`') || (c =='~'))
	{
		SetState(0);
		return;
	}
	if((c == '\b') && (m_input_buffer.size()))
	{
		m_input_buffer.resize(m_input_buffer.size()-1);
		return;
	}
	if(c == 0) return;
	m_input_buffer += c;
}