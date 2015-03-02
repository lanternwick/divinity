#pragma warning(disable:4786)

#include "log_stream.h"
#include "client.h"
#include "SYSWindow.h"
#include <string>
#include "string_tokenizer.h"
#include "gwen.h"


/* notes

  when attacking:
    melee units attack closest target.  If target that is higher level attacks unit, attack that unit.
	ranged units attack farthest target, within range.  If ranged unit attacks unit, attack attacking unit.

  */

LRESULT __stdcall MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);

}

int __stdcall WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	_log l;
	l.set_debug_level(1);
	IObject::set_log(&l);
	l.write("dgssdfgsdgsdfg\n", 0);
	std::string config("environ.xml");
	std::string c_config("client.xml");
	string_tokenizer<> s(std::string(lpCmdLine), std::string(" "));
	for(int i = 0; i < s.size(); ++i)
	{
		// parse options
		if((s.token(i).at(0) == '-') && (s.token(i).at(1) != '-'))
		{
			// short options
			for(int j = 1; j < s.token(i).length(); ++j)
			{
				switch(s.token(i).at(j))
				{
				case 'c': // client config
					c_config = s.token(i+j);
					break;
				case 'C': // config
					config = s.token(i+j);
					break;
				default:
					break;
				}
			}
		}
	}

	file_g_input<char> fi(config);
	g_parser<char> p(fi);
	g_block<char> r = p.root();
	g_block<char> win = r.block(std::string("window"));
	int x, y, cx, cy;
	GSS(win, "x") >> x;
	GSS(win, "y") >> y;
	GSS(win, "cx") >> cx;
	GSS(win, "cy") >> cy;

	SYSWindow window;
	window.Create(x, y, cx, cy, "TOW");
	ShowWindow(window.hWnd(), SW_SHOWNORMAL);
	UpdateWindow(window.hWnd());

	client c;
	c.init(c_config, window.hWnd());
	MSG msg;
	while(true)
	{
		if(PeekMessage(&msg, window.hWnd(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(c.step())
		{
			break;
		}
	}

	c.term();

	GSSI(win, "x", x);
	GSSI(win, "y", y);
	GSSI(win, "cx", cx);
	GSSI(win, "cy", cy);
	file_g_output<char> fo(config);
	g_deparser<char> d(fo, r);
	return 0;
}



