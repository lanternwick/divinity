#include "stdafx.h"
#include "..\DivCore\Core.h"
#include "TimesOfWar_Mod.h"
#include "pump_thread.h"
#include "render_block.h"
#include "update_block.h"
#include <crtdbg.h>
#include "Terrain_Tile.h"
#include "..\DivGraph\AnimatedTexture.h"
#include "..\DivUI\DivUI.h"
#include "..\DivUI\UIConsoleWnd.h"

TimesOfWar_Mod::TimesOfWar_Mod()

{
	Core::Function_Enter("TimesOfWar::Constructor()");
	m_device = NULL;
	m_factory = NULL;
	m_ui = NULL;
	m_map = NULL;
	Core::Function_Exit();
}

TimesOfWar_Mod::~TimesOfWar_Mod()
{
	Core::Function_Enter("TimesOfWar::Destructor()");
	Core::Function_Exit();

}

INT TimesOfWar_Mod::Init(IDevice* device, char* cmdLine)
{
	Core::Function_Enter("TimesOfWar_Mod::Init()");
	Core::WriteLog("{------ Times of War ------}\n", 0);
	//initialize rendering
	m_device = device;
	singleton<IDevice*> sdevice(m_device);
	RESULT r = m_device->Create(hWnd, TRUE, 800, 600);
	if(RFAILED(r))
	{
		return -1;
	}
	m_device->SetRenderState(RS_ALPHABLENDENABLE, 1);
	m_device->SetRenderState(RS_AMBIENT, ARGB(1.0f, 0.8f, 0.8f, 0.8f));
	m_factory = DivGraph_GetFactory();
	m_factory->SetDevice(m_device);
	singleton<IGraphObjFactory*> sfactory(m_factory);

	// initialize user interface module
	m_ui = new UserInterface;
	
	singleton<UserInterface*> sui(m_ui);

	m_ui->Input().CreateDevice(hWnd); //Initialize(hWnd, m_device, m_factory);
	DivUI_InitEx(true, m_ui);
	m_ui->Input().SetInputHandler(&m_input);
	m_ui->Fonts().SetDevice(m_device);
	m_ui->Fonts().SetFactory(m_factory);
	m_ui->Fonts().LoadFont("fonts\\verdana.font", moniker("default"));
	
	singleton<UICommandMap*> scommand_map(&m_command_map);

	// set up console
	singleton<console*> sconsole(&tow_console);

	//--- quit()
	m_console_functors.push_back(new quit_functor(&m_running));
	tow_console.add_function(std::string("function quit()"), m_console_functors.back());

	//--- console()
	m_console_functors.push_back(new console_s_functor);
	tow_console.add_function(std::string("function console()"), m_console_functors.back());

	//--- bind(key), bind(key, action)
	m_console_functors.push_back(new bind_functor(&m_command_map));
	tow_console.add_function(std::string("function bind(string, string)"), m_console_functors.back());
	tow_console.add_function(std::string("function bind(string)"), m_console_functors.back());

	//--- map(mapname)
	m_console_functors.push_back(new map_functor(&m_map));
	tow_console.add_function(std::string("function map(string)"), m_console_functors.back());

	//--- move(direction descriptor)
	m_console_functors.push_back(new move_functor(&m_camera_cont));
	tow_console.add_function(std::string("function move(string)"), m_console_functors.back());

	//--- exec(filename)
	m_console_functors.push_back(new exec_functor);
	tow_console.add_function(std::string("function exec(string)"), m_console_functors.back());

	//--- alias(alias name, alias action)
	m_console_functors.push_back(new alias_functor);
	tow_console.add_function(std::string("function alias(string, string)"), m_console_functors.back());

	//--- developer(debug level)
	m_console_functors.push_back(new developer_functor);
	tow_console.add_function(std::string("function developer(int)"), m_console_functors.back());

	//--- cmd_list()
	m_console_functors.push_back(new cmd_list_functor);
	tow_console.add_function(std::string("function cmd_list()"), m_console_functors.back());

	//--- toggle_shadows()
	m_console_functors.push_back(new shadows_functor);
	tow_console.add_function(std::string("function toggle_shadows()"), m_console_functors.back());

	//--- wireframe()
	m_console_functors.push_back(new wireframe_functor(m_device));
	tow_console.add_function(std::string("function wireframe()"), m_console_functors.back());

	//--- solid()
	m_console_functors.push_back(new solid_functor(m_device));
	tow_console.add_function(std::string("function solid()"), m_console_functors.back());

	//--- move(move definition)
	m_console_functors.push_back(new move_functor(&m_camera_cont));
	tow_console.add_function(std::string("function move(string)"), m_console_functors.back());


	m_command_map.Bind(DIV_KEY_GRAVE, std::string("console"));

	UIConsoleWnd* con = new UIConsoleWnd("console");
	
	con->Create(m_device, m_factory, 0.0f, 0.0f, 1.0f, 0.0f, 0x33ffffff, moniker("none"), NULL);
	window_message m;
	m.message = CONMSG_SETCONSOLE;
	m.ulong_in[0] = (unsigned long)&tow_console;
	con->send_message(m);

	m_ui->Windows().AttachWindow(con);

	m_map = new TOW_Map;
	
	m_world = new TOW_World;
	singleton<TOW_World*> sworld(m_world);
	m_world->SetDevice(m_device);
	m_world->SetConsole(&tow_console);
	m_world->SetMap(m_map);
	
	m_map->SetDevice(m_device);
	
	m_camera.pos(VECTOR(10, 10, 10));
	m_camera.at(VECTOR(10, 0, 19));
	m_camera.up(VECTOR(0.0f, 1.0f, 0.0f));
	m_camera.set_device(m_device);
	m_camera.near_pl(0.1f);
	m_camera.far_pl(100.0f);
	m_camera.aspect(1.0f);
	m_camera.fov(3.14159f / 4.0f);
	singleton<CCamera*> scam(&m_camera);
	m_camera_cont.Attach(&m_camera);
	m_camera_cont.Velocity(6.0f);
	m_camera_cont.LimitCamera(true);
	rect<float> limit_rect;
	limit_rect.left = 0; limit_rect.right = 100;
	limit_rect.top = 100; limit_rect.bottom = 0;
	m_camera_cont.LimitRect(limit_rect);
	PLANE limit_plane = {0, 1, 0, 0};
	m_camera_cont.LimitPlane(limit_plane);
	singleton<TOW_CameraController*> scam_cont(&m_camera_cont);
	m_map->SetCamera(&m_camera);
	m_camera_cont.Render();
	TOW_GameObject::SetDevice(m_device);
	Terrain_Tile::SetDevice(m_device);
	Terrain_Tile::SetFactory(m_factory);
	lua_tow_init();

	Core::Function_Exit();
	
	return 0;
}

INT TimesOfWar_Mod::Run(IDevice* device, char* cmdLine)
{
	
	Core::Function_Enter("TimesOfWar_Mod::Run()");
	string_tokenizer<> c(std::string(cmdLine), std::string(" "));

	tow_console.exec_command(std::string("exec \"scripts\\config.tcs\""));
	
	update_block updater;
	//updater.step();

	/*m_map->Load("maps/solid.twm");
	m_map->Create(0, 0, 0);
	m_device->Clear(0xffffffff);
	m_device->BeginScene();
	m_map->Render();
	m_device->EndScene();
	m_device->BufferDump("solid.dump");
	m_map->Destroy();
	m_map->Load("maps/wireframe.twm");
	m_map->Create(0, 0, 0);
	m_device->SetRenderState(RS_FILLMODE, FILL_WIREFRAME);
	m_device->Clear(0xffffffff);
	m_device->BeginScene();
	m_map->Render();
	m_device->EndScene();
	m_device->BufferDump("wireframe.dump");
	return 0;*/
	

	render_block renderer;
	//renderer.begin(THREAD_CREATE_SUSPENDED);
	
	//updater.begin(THREAD_CREATE_SUSPENDED);

	m_running = true;

	
	

	MSG msg;
	while(m_running)
	{
		if(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Core::BeginFrame();
		updater.step();
		renderer.step();
		Core::EndFrame();
	}
	


	Core::Function_Exit();
	
	
	return 0;
}

INT TimesOfWar_Mod::Cleanup()
{
	
	Core::Function_Enter("TimesOfWar_Mod::Cleanup()");

	
	if(m_map)
	{
		m_map->Destroy();
		m_map->ClearObjects(true);
		delete m_map;
		m_map = NULL;
	}
	while(m_console_functors.size())
	{
		delete m_console_functors.back();
		m_console_functors.pop_back();
	}
	
	if(m_ui)
	{
		delete m_ui;
		m_ui = NULL;
	}
	if(m_world)
	{
		delete m_world;
		m_world = NULL;
	}
	
	lua_tow_close();
	
	if(m_device)
	{
		m_device->Cleanup();
		//delete m_device;
		m_device = NULL;
	}
	
	Core::Function_Exit();
	
	return 0;
}
