#include "client.h"
#include "D3DDevice.h"
#include "UIInputHandler.h"
#include "UIConsoleWnd.h"
#include "UITitlebar.h"
#include "UIButton.h"
#include "UIScrollbar.h"
#include "UITextbox.h"
#include "UICheckbox.h"
#include "UIDisplaybox.h"

client::client()
{

}

client::~client()
{

}

int client::init(const std::string& config, HWND hWnd)
{
	m_config = config;
	std::string cursor_tex_f("textures/cursor.texture");
	m_device = new D3DDevice;
	m_device->Create(hWnd, TRUE, 640, 480);
	m_ui.Input().CreateDevice(hWnd);
	m_ui.Fonts().SetDevice(m_device);
	m_ui.Fonts().LoadFont("fonts/default.font", moniker("default"));
	UIInputHandler* uiih = new UIInputHandler;
	uiih->SetUI(&m_ui);
	m_ui.Input().SetInputHandler(uiih, true);
	UITitlebar* uitb = NULL;
	uitb = new UITitlebar;
	
	UIConsoleWnd* uicw = NULL;
	uicw = new UIConsoleWnd("console");
	uicw->Create(m_device, 0.3f, 0.2f, 0.4f, 0.3f, 0x99001dc1, NULL, NULL);
	uicw->set_font(m_ui.Fonts().GetFont(moniker("default")));
	uitb->Create(m_device, 15, 0xffff0000, 0x33ff0000, NULL, m_ui.Fonts().GetFont(moniker("default")), uicw);
	window_message m;
	m.message = CONMSG_EXPAND;
	uicw->send_message(m);
	m.message = CONMSG_SETCONSOLE;
	m.ulong_in[0] = (unsigned long)&m_con;
	uicw->send_message(m);
	m_ui.Windows().AttachWindow(uitb);
	UICursor* cursor = NULL;
	cursor = new UICursor;
	cursor->Create(m_device, &m_ui.Input());
	ITexture* cursortex = NULL;
	m_device->LoadTexture(cursor_tex_f.c_str(), cursortex);
	if(cursortex)
	{
		cursor->SetCursor(cursortex);
	}
	ITexture* grass_tex = NULL;
	m_device->LoadTexture("textures/grass64.texture", grass_tex);
	m_tm.AttachTexture(grass_tex);
	unsigned char* buff = new unsigned char[32*32];
	memset(buff, 0, 32*32);
	unsigned char* buff2 = new unsigned char[32*32];
	memset(buff2, 0, 32*32);
	m_tm.CreateMap(32, buff);
	m_hm.CreateMap(32, buff2);
	Terrain_Tile::SetHeightMap(&m_hm);
	Terrain_Tile::SetTextureMap(&m_tm);
	Terrain_Tile::SetDevice(m_device);
	m_map.SetDevice(m_device);
	m_map.Create(32, 32, 8.0f);
	m_map.SetCamera(&m_cam);
	m_cam.aspect(1.0f);
	m_cam.fov(3.14159f/4);
	m_cam.far_pl(100.0f);
	m_cam.near_pl(0.1f);
	m_cam.up(VECTOR(0.0f, 1.0f, 0.0f));
	m_cam.pos(VECTOR(10.0f, 10.0f, 10.0f));
	m_cam.at(VECTOR(10.0f, 0.0f, 19.0f));
	m_cam.set_device(m_device);
	m_ui.Windows().AttachWindow(cursor);
	m_device->SetRenderState(RS_ALPHABLENDENABLE, 1);
	m_device->SetRenderState(RS_FILLMODE, FILL_SOLID);
	return 0;
}

int client::step()
{
	if(do_net() == -1)
	{
		return -1;
	}
	if(do_input() == -1)
	{
		return -1;
	}
	if(update_world() == -1)
	{
		return -1;
	}
	if(render() == -1)
	{
		return -1;
	}
	
	return 0;
}

int client::term()
{
	m_ui.Release();
	m_map.Destroy();
	delete m_device;
	return 0;
}

int client::render()
{
	m_device->Clear(0xff0000ff);
	m_device->BeginScene();
	
	m_map.Render();
	m_ui.Render();

	m_device->EndScene();
	m_device->Flip();
	return 0;
}

int client::update_world()
{
	m_ui.Think();
	
	m_cam.set();
	return 0;
}

int client::do_input()
{
	m_ui.Input().Think();
	if(m_ui.Input().GetMouseButton(1))
	{
		return -1;
	}
	return 0;
}

int client::do_net()
{

	return 0;
}