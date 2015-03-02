#ifndef UIDISPLAYBOX_H
#define UIDISPLAYBOX_H

#include "UIBaseWindow.h"
#include "UIScrollbar.h"
#include "Font.h"

class UIDisplaybox : public UIBaseWindow
{
public:
	UIDisplaybox();
	~UIDisplaybox();

	int Create(IDevice* device, float xp, float yp, float cxp, float cyp, unsigned long color, 
						 unsigned long t_color, Font* fnt, ITexture* texture, ITexture* sb_arrow, ITexture* sb_thumb,
						 const std::string& text, UIWindow* parent);

	RESULT Render();

	void send_message(window_message& msg);
private:
	std::string m_text;
	UIScrollbar* m_sb;
	Font* m_font;
	int m_offset;
	int m_lines;
	int m_max_line_disp;
	unsigned long m_t_color;
};

#endif
