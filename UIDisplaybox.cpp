#include "UIDisplaybox.h"

UIDisplaybox::UIDisplaybox()
{

}

UIDisplaybox::~UIDisplaybox()
{

}

int UIDisplaybox::Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, 
						 unsigned long t_color, Font* fnt, ITexture* texture, ITexture* sb_arrow, ITexture* sb_thumb,
						 const std::string& text, UIWindow* parent)
{
	UIBaseWindow::Create(device, xp, yp, cxp, cyp, color, texture, parent);
	m_text = text;
	int c = 0;
	for(int i = 0; i < m_text.length(); ++i)
	{
		if(m_text[i] == '\n') ++c;
	}
	m_font = fnt;
	m_lines = c;
	m_max_line_disp = m_size.cy / fnt->GetCellSize().cy;
	m_offset = 0;
	m_t_color = t_color;
	m_sb = new UIScrollbar;
	m_sb->Create(device, 0.95f, 0.0f, 0.05f, float(m_size.cy)/m_size.cx, texture, sb_arrow, sb_thumb, color, m_max_line_disp, m_lines, this);
	m_sb->AttachTo(this);
	return 0;
}

RESULT UIDisplaybox::Render()
{
	UIBaseWindow::Render();
	std::string o;
	int offset = m_offset;
	int lines = m_max_line_disp;
	if(m_font)
	{
		char c;
		long i = 0;
		while((offset))
		{
			if(m_text[i] == '\n')
				--offset;
			++i;
		}
		for(;lines != 0, i != m_text.length(); ++i)
		{
			
			o += m_text[i];
			if(m_text[i] == '\n')
				--lines;
		}
		RECT r = { sx(), sy(), sx()+m_size.cx, sy()+m_size.cy };
		m_font->DrawText(o.c_str(), r, NULL, 1.0f, m_t_color);
	}
	return OK;
}

void UIDisplaybox::send_message(window_message& msg)
{
	switch(msg.message)
	{
	case SCROLLUP:
		if(m_offset)
		{
			--m_offset;
		}
		break;
	case SCROLLDN:
		if(m_offset != m_lines-1)
		{
			++m_offset;
		}
		break;
	case SBVALCNG:
		if((msg.ulong_in[0] > 0) && (msg.ulong_in[0] < m_lines))
		{
			m_offset = msg.ulong_in[0];
		}
		break;

	default:
		break;
	}
}