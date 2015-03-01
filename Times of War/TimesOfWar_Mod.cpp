#include "stdafx.h"
#include "TimesOfWar_Mod.h"
#include "TOW_InputHandler.inc"

//CEngine* TimesOfWar_Mod::engine = NULL;
//IDevice* TimesOfWar_Mod::myDevice = NULL;
/*CScene* TimesOfWar_Mod::currScene = NULL;
TOW_Map* TimesOfWar_Mod::map = NULL;
float TimesOfWar_Mod::xPos = 10.0f;
float TimesOfWar_Mod::zPos = 10.0f;
float TimesOfWar_Mod::yAdd = 10.0f;
bool TimesOfWar_Mod::running = false;
UserInterface* TimesOfWar_Mod::ui = NULL;
console TimesOfWar_Mod::tow_console;
std::string TimesOfWar_Mod::console_prompt;
TOW_ObjectManager<> TimesOfWar_Mod::m_objectMan;
TOW_OutputManager<> TimesOfWar_Mod::m_guOutputMan;*/



struct Mesh : public IRenderable, public IConsoleBind
{
	StaticMesh* mesh;
	VECTOR pos;
	float rotate;
	IDevice* myDevice;
	ITexture* tex;
	bool render;
	RESULT Render()
	{
		MATRIX m, m2, m3;
		if(!render) return OK;
		if(tex)
			tex->Set(0);
		MatrixMatrixRotationY(&m2, rotate);
		MatrixMatrixTranslation(&m, pos.x, pos.y, pos.z);
		MatrixMatrixMultiply(&m3, &m2, &m);
		myDevice->SetTransform(T_WORLD, &m3);
		mesh->Render();return OK;
	}
	void Dispatch(std::string& function_name, console* c, class_data* _this, const class_def* def, std::vector<variant>& params)
	{
		if(function_name == "hide")
		{
			render = false;
			return;
		}
		if(function_name == "show")
		{
			render = true;
		}
	}
	
	Mesh()
	{
		tex = NULL;
		render = true;
	}
};

static VOID DoEvents(HWND hWnd)
{
	MSG msg;
	if(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}

TimesOfWar_Mod::TimesOfWar_Mod()
{
	Core::Function_Enter("TimesOfWar::Constructor()");
	myDevice = NULL;
	currScene = NULL;
	map = NULL;
	xPos = 20.0f;
	zPos = 20.0f;
	yAdd = 15.0f;
	running = false;
	ui = NULL;
	Core::SetDebugLogLevel(10);
	Core::Function_Exit();
}

TimesOfWar_Mod::~TimesOfWar_Mod()
{
	Core::Function_Enter("TimesOfWar::Destructor()");
	Core::Function_Exit();

}

RESULT TimesOfWar_Mod::DrawFrame(CFrame* frameToRender)
{
	Core::Function_Enter("TimesOfWar::DrawFrame()");

	frameToRender->Clear(0xff000000);
	frameToRender->Render();
	frameToRender->Flip();

	Core::Function_Exit();
	return OK;
}

RESULT TimesOfWar_Mod::UpdateFrame(CScene* sceneToUpdate)
{
	Core::Function_Enter("TimesOfWar::UpdateFrame()");
	
	Core::Function_Exit();
	return OK;
}

INT TimesOfWar_Mod::Init(IDevice* device, char* cmdLine)
{
	log_stream<0> ls;
	
	IGraphObjFactory* factory;
	singleton<TOW_World*> s_world(new TOW_World);
	world = singleton<TOW_World*>::instance();
	m_current_sel = new TOW_Selector;
	m_p_map = new TOW_PathingMap;
	m_player = new TOW_Player;
	m_player->team = 0;
	m_player->color = 0xff0000ff;
	lua_tow_init();
	world->SetPathingMap(m_p_map);
	myDevice = device;
		ls << "Startup...\n";

	//core = Core::Instance();
		ls << "Initializing D3D... ";
	
	
		if(myDevice)
		{
			ls << "Success!\n";
		}
		else
		{
			ls << "Failure!\n";
			return -1;
		}
	factory = DivGraph_GetFactory();
	myDevice->Create(hWnd, 1, 800, 600);
	singleton<IDevice*> s_device(myDevice);
	singleton<IGraphObjFactory*> s_factory(factory);
	singleton<console*> s_console(&tow_console);

	factory->SetDevice(myDevice);
	myDevice->SetRenderState(RS_FILLMODE, FILL_SOLID);
	myDevice->SetRenderState(RS_ALPHABLENDENABLE, TRUE);
	myDevice->SetRenderState(RS_CULLMODE, CULL_NONE);

	
	
	MenuScene = new CScene;
	GameScene = new CScene;
	MenuScene->SetDevice(myDevice);
	GameScene->SetDevice(myDevice);

	TOW_Selector::SetWorld(world);
	
	TOW_Selector::SetDevice(myDevice);
	TOW_Selector::SetFactory(factory);
	TOW_Selector::SetPlayer(m_player);
	TOW_GameObject::SetDevice(myDevice);
	TOW_GameObject::SetFactory(factory);
	TOW_SelectBox::SetWorld(world);
	
	TOW_SelectBox::SetDevice(myDevice);
	TOW_SelectBox::SetFactory(factory);


	world->SetConsole(&tow_console);
	world->SetDevice(myDevice);
	TOW_SelectBox::SetScene(GameScene);
	
	DivUserInterface_Init(true);
	ui = UserInterface::Instance();
	ui->Initialize(hWnd, myDevice, factory);
	
	map = new TOW_Map;
	map->Identifier()() = "map";
	map->SetDevice(myDevice);

	singleton<TOW_ObjectManager*> sobjman(map);
	singleton<TOW_ObjectManager*>::instance()->SetDevice(myDevice);

	ls << "Configuring Console... ";
	static wireframe_functor wf(myDevice);
	static solid_functor sf(myDevice);
	static quit_functor qf;
	qf.set_running(&running);
	static set_prompt_functor spf;
	spf.set_prompt(&console_prompt);
	static clear_functor cf;
	static exec_functor exef;
	static update_object_functor upf(world);
	static mem_stat_functor msf;
	static bind_functor bindf(&m_commandmap);
	static console_s_functor conf;
	static seq_functor seqf;
	static start_sel_functor ssf(myDevice, m_current_sel, GameScene, map, &m_sel_box);
	static end_sel_functor esf(myDevice, m_current_sel, GameScene, map, world, &m_sel_box);
	static x_move_functor xmovef(&m_cam_dir);
	static z_move_functor zmovef(&m_cam_dir);
	static x_stop_functor xstopf(&m_cam_dir);
	static z_stop_functor zstopf(&m_cam_dir);
	static context_functor contf(myDevice, m_current_sel, GameScene, map, world);
	static main_menu_functor mmf(&m_mode);
	static name_functor nf(m_player);
	static connect_functor connectf(&net, m_player);
	static say_functor sayf(&net);
	static whisper_functor whisperf(&net);
	static join_functor joinf(&net);
	static map_functor mapf(map);

	tow_console.add_function(std::string("function wireframe()"), &wf);
	tow_console.add_function(std::string("function solid()"), &sf);
	tow_console.add_function(std::string("function quit()"), &qf);
	tow_console.add_function(std::string("function quit(int)"), &qf);
	tow_console.add_function(std::string("function set_prompt(string)"), &spf);
	tow_console.add_function(std::string("function clear()"), &cf);
	tow_console.add_function(std::string("function mem_stat()"), &msf);
	tow_console.add_function(std::string("function exec(string)"), &exef);
	tow_console.add_function(std::string("function update_object(string)"), &upf);
	tow_console.add_function(std::string("function bind(string)"), &bindf);
	tow_console.add_function(std::string("function bind(string, string)"), &bindf);
	tow_console.add_function(std::string("function console()"), &conf);
	tow_console.add_function(std::string("function seq(string)"), &seqf);
	tow_console.add_function(std::string("function start_sel()"), &ssf);
	tow_console.add_function(std::string("function +sel()"), &ssf);
	tow_console.add_function(std::string("function start_sel(float, float)"), &ssf);
	tow_console.add_function(std::string("function end_sel()"), &esf);
	tow_console.add_function(std::string("function -sel()"), &esf);
	tow_console.add_function(std::string("function +x_move(string)"), &xmovef);
	tow_console.add_function(std::string("function +z_move(string)"), &zmovef);
	tow_console.add_function(std::string("function -x_move(string)"), &xstopf);
	tow_console.add_function(std::string("function -z_move(string)"), &zstopf);
	tow_console.add_function(std::string("function main_menu()"), &mmf);
	tow_console.add_function(std::string("function name(string)"), &nf);
	tow_console.add_function(std::string("function connect(string)"), &connectf);
	tow_console.add_function(std::string("function say(string)"), &sayf);
	tow_console.add_function(std::string("function whisper(string, string)"), &whisperf);
	tow_console.add_function(std::string("function join(string)"), &joinf);
	tow_console.add_function(std::string("function map(string)"), &mapf);
	
	
	tow_console.add_function(std::string("function end_sel(float, float)"), &esf);
	tow_console.add_function(std::string("function context()"), &contf);
	tow_console.add_function(std::string("function set_game_speed(float)"), &game_speed_functor());
	//tow_console.add_object_class(std::string("class IMesh{ function \ashow()\a; function \ahide()\a; function \aset_skin(string)\a; function \aget_info()\a; function \aset_pos(float, float)\a; function \aget_pos()\a; function \agoto(float, float, float)\a;};"));
	std::ifstream classes;
	classes.open("object_templates\\unit.class");
	tow_console.add_object_class_from_file(classes);
	classes.close();
	std::ifstream snowball;
	snowball.open("object_templates\\snowball.class");
	tow_console.add_object_class_from_file(snowball);
	snowball.close();

	tow_console.set_security_level(10);
	ls << "Success!\n";


	game.SetName("some game");
	game.SetVersion("1.0");
	session.SetGame(game);
	session.SetLocked(false);
	session.SetPassword("");
	//session.SetPort(5600);
	session.SetMaximum(4);

	net.Initialize(&protocol);
	net.Select(DN_NETWORK_IP);

	MailBox::SetNetwork(&net);

	tow_console.exec_command(std::string("exec \"scripts\\config.tcs\""));

	
	Terrain_Tile::SetDevice(myDevice);
	Terrain_Tile::SetFactory(factory);



	return 0;
}

INT TimesOfWar_Mod::Run(IDevice* device, char* cmdLine)
{
	
	
	log_stream<0> ls;
	
	
	CFrame* currFrame;
	ITexture* grass, * rock, * cursor, * pine, * wood,
			* title_bg, * line_tex, * selind, * sel_corner, * christmas_tree, * tos,
			* s_but_tex, * q_but_tex;
	
	Core* core;
	UIWindow* banner;
	UIConsoleWnd* con;
	UIGameStartButton* start_but;
	UIGameQuitButton* quit_but;
	Mesh m;
	TOW_InputHandler input;
	Core::Function_Enter("TimesOfWar::Run()");

	
	

	UserInterface::Instance()->SetInputHandler(&input);
	
	currFrame = new CFrame;
	currFrame->SetDevice(myDevice);
	
	
	
	
	singleton<IGraphObjFactory*>::instance()->LoadTexture("textures\\cursor.texture", cursor, "cursor");

		ui->SetCursor(cursor);
	world->SetMap(map);
	map->Load("maps\\2.data");
	
	Terrain_Tile::SetHeightMap(map->HeightMap());
	Terrain_Tile::SetTextureMap(map->TextureMap());
	TOW_SelectIndicator::SetHeightMap(map->HeightMap());
	TOW_GameObject::SetHeightMap(map->HeightMap());

	
	map->SetScene(GameScene);
	
	
	
	map->Create(2, 2, 39.0f);
	
	input.SetTOWMod(this);

	
	std::ifstream in("maps\\pathing_map.map", std::ios::binary);
	m_p_map->generate_pathing(in, map->HeightMap());
	in.close();
	

	
	//TOW_ObjectManager manager_001234;
	GameScene->AddObject(map);

	
	TOW_GameObjectOutput::SetHeightMap(map->HeightMap());

	
	myDevice->SetRenderState(RS_AMBIENT, 0xff888888);

	console_prompt = "> ";

	

	ui->LoadFont("fonts\\verdana.bold.font", moniker("default"));

	Core::RegisterMediaObject(new UIWindow("someWindow"));
	banner = (UIWindow*)Core::GetMediaObject(moniker("someWindow"));
	banner->SetTextures((ITexture*)Core::GetMediaObject(moniker("tos")));
	banner->Create(myDevice, singleton<IGraphObjFactory*>::instance(), 0.25f, 0.0f, 0.5f, 0.5f, 0xffffffff, 0xffffffff, false, false, 42.0f, 0xffffffff, 1, 0xff000000, 20.0f);
	banner->SetFont(moniker("default"));
	banner->SetCaption(std::string("Window!"));
	banner->Think();
	ui->AttachWindow(banner);
	MenuScene->AddObject(banner);

	Core::RegisterMediaObject(new UIConsoleWnd("console"));
	con = (UIConsoleWnd*)Core::GetMediaObject(moniker("console"));
	con->SetUIPtr(ui);
	con->SetTextColor(0xff000000);
	con->Create(myDevice, singleton<IGraphObjFactory*>::instance(), 0.0f, 0.0f, 1.0f, 0.0f, 0x33ffffff, 0xffffffff, false, true, 42.0f, 0xffffffff, 1, 0xff000000, 20.0f);
	con->SetFont(moniker("default"));
	con->SetCaption(std::string("Console"));
	con->SetTextures(NULL, NULL);

	con->SetPrompt(std::string("> "));
	ui->AttachWindow(con);
	//con->AddChildWindow(window);
	

	Core::RegisterMediaObject(new UIGameStartButton("start_button"));
	start_but = (UIGameStartButton*)Core::GetMediaObject(moniker("start_button"));
	start_but->Create(myDevice, singleton<IGraphObjFactory*>::instance(), 0.25f, 0.65f, 0.5f, 0.1f, 0xffffffff, 0xffffffff, false, 0xff000000, 2.0f, NULL);
	start_but->SetFont(moniker("default"));
	start_but->SetCaption(std::string(" "));
	start_but->SetModePointer(&m_mode);
	start_but->SetTextures((ITexture*)Core::GetMediaObject(moniker("sb")));
	ui->AttachWindow(start_but);
	MenuScene->AddObject(start_but);
	Core::RegisterMediaObject(new UIGameQuitButton("quit_button"));
	quit_but = (UIGameQuitButton*)Core::GetMediaObject(moniker("quit_button"));
	quit_but->Create(myDevice, singleton<IGraphObjFactory*>::instance(), 0.25f, 0.75f, 0.5f, 0.1f, 0xffffffff, 0xffffffff, false, 0xff000000, 2.0f, NULL);
	quit_but->SetFont(moniker("default"));
	quit_but->SetCaption(std::string(" "));
	quit_but->SetRunningPointer(&running);
	quit_but->SetTextures((ITexture*)Core::GetMediaObject(moniker("qb")));
	ui->AttachWindow(quit_but);
	MenuScene->AddObject(quit_but);
	/*Core::RegisterMediaObject(new UIButton("quit_button"));
	but = (UIButton*)Core::GetMediaObject(moniker("somebutton"));
	but->Create(myDevice, factory, 0.1f, 0.1f, 0.5f, 0.25f, 0x88ffffff, 0xffffffff, true, 0xff000000, 2.0f, window);
	but->SetFont(moniker("default"));
	but->SetCaption(std::string("A Button!"));
	ui->AttachWindow(but);
	window->AddChildWindow(but);*/
	con->SetState(0);
	singleton<TOW_ObjectManager*>::instance()->RegisterWindow(con);
	MenuScene->AddObject(con);
	singleton<TOW_ObjectManager*>::instance()->RegisterObject(m_current_sel);

	CCamera thisCam;
	// define camera
	thisCam.fAspect = 1.0f;
	thisCam.fFov = 3.14159f / 4.0f;
	thisCam.fFar = 1000.0f;
	thisCam.fNear = 0.1f ;
	thisCam.vecAt = VECTOR(xPos,0.0f, zPos+10.0f);
	thisCam.vecPos = VECTOR(xPos, 
							yAdd/*map->HeightMap()->GetHeight(xPos, zPos)*/,
							zPos);
	thisCam.vecUp = VECTOR(0.0f, 1.0f, 0.0f);
	// set camera
	GameScene->SetCamera(&thisCam);

	//TOW_ObjectManager::RegisterObject(ui);
	//myDevice->Reset(hWnd, 0, 640, 480);
	myDevice->SetRenderState(RS_FILLMODE, FILL_SOLID);
	myDevice->SetRenderState(RS_ALPHABLENDENABLE, TRUE);
	//myDevice->SetRenderState(RS_CULLMODE, CULL_NONE);
	myDevice->SetRenderState(RS_AMBIENT, 0xff888888);

	GameScene->AddObject(ui);
	MenuScene->AddObject(ui);
	currScene = MenuScene;
	running = true;
	m_mode = MENU;
	CCamera cCam;
	CCamera lastCam;
	//m_commandmap.Bind(DIV_KEY_ESCAPE, std::string("quit"));
	m_commandmap.Bind(DIV_KEY_GRAVE, std::string("console"));
	while(running)
	{
		Think();
		core->BeginFrame();
		DoEvents(hWnd);
		switch(m_mode)
		{
		case GAME:
			currScene = GameScene;
			ui->UnselectWindow();
			banner->Deactivate();
			start_but->Deactivate();
			quit_but->Deactivate();
			lastCam = thisCam;
			thisCam.vecPos.z+=m_cam_dir.z*6.0f*Core::GetTimeDelta();
			//thisCam.vecPos.z-=6.0f*core->GetTimeDelta();
			thisCam.vecAt = VECTOR(thisCam.vecPos.x,0.0f, thisCam.vecPos.z+yAdd);
			//thisCam.vecPos = VECTOR(xPos, 
			//						yAdd/*map->HeightMap()->GetHeight(xPos, zPos)*/,
			//						zPos);
			thisCam.vecUp = VECTOR(0.0f, 1.0f, 0.0f);
			
			
			currScene->SetCamera(&thisCam);
			if(map->ViewWithinBounds(&thisCam))
			{
				
				//thisCam = cCam;
				// set camera
				zPos = thisCam.vecPos.z;
				//xPos = thisCam.vecPos.x;
			}
			else
			{
				thisCam = lastCam;
				yAdd = lastCam.vecPos.y;
				currScene->SetCamera(&lastCam);
			}
			
			lastCam = thisCam;
			//if(ui->GetKeyState(DIV_KEY_RIGHT)) 
			thisCam.vecPos.x+=m_cam_dir.x*6.0f*core->GetTimeDelta();
			//if(ui->GetKeyState(DIV_KEY_LEFT)) 
			//thisCam.vecPos.x-=6.0f*core->GetTimeDelta();
			thisCam.vecAt = VECTOR(thisCam.vecPos.x,0.0f, thisCam.vecPos.z+yAdd);
			//thisCam.vecPos = VECTOR(xPos, 
			//						yAdd/*map->HeightMap()->GetHeight(xPos, zPos)*/,
			//						zPos);
			thisCam.vecUp = VECTOR(0.0f, 1.0f, 0.0f);
			
			
			currScene->SetCamera(&thisCam);
			if(map->ViewWithinBounds(&thisCam))
			{
				
				//thisCam = cCam;
				// set camera
				zPos = thisCam.vecPos.z;
				xPos = thisCam.vecPos.x;
			}
			else
			{
				thisCam = lastCam;
				yAdd = lastCam.vecPos.y;
				currScene->SetCamera(&lastCam);
			}
			//cCam = thisCam;
			
			//engine->EUserInterface()->Render();
			world->Think();
			singleton<TOW_ObjectManager*>::instance()->Think();
			break;
		case MENU:
			currScene = MenuScene;
			banner->Activate();
			start_but->Activate();
			quit_but->Activate();
			break;
		}
		ui->Think();

		UpdateFrame(currScene);
		currFrame = currScene->Process(currFrame);
		DrawFrame(currFrame);
		
		core->EndFrame();
	}
	ui->DetachWindow(con);
	Core::RemoveMediaObject(moniker("grass"), true);
	Core::RemoveMediaObject(moniker("snow"), true);
	Core::RemoveMediaObject(moniker("cursor"), true);
	Core::RemoveMediaObject(moniker("tower"), true);
	Core::RemoveMediaObject(moniker("tree"), true);
	Core::RemoveMediaObject(moniker("snowman"), true);
	Core::RemoveMediaObject(moniker("snowball"), true);
	Core::RemoveMediaObject(moniker("someWindow"), true);
	Core::RemoveMediaObject(moniker("console"), true);
	Core::RemoveMediaObject(moniker("edgeC"), true);
	Core::RemoveMediaObject(moniker("edgeV"), true);
	Core::RemoveMediaObject(moniker("edgeH"), true);
	Core::RemoveMediaObject(moniker("pine"), true);
	Core::RemoveMediaObject(moniker("wood"), true);
	Core::RemoveMediaObject(moniker("land_ship"), true);
	Core::RemoveMediaObject(moniker("title_bg"), true);
	Core::RemoveMediaObject(moniker("window_bg"), true);
	Core::RemoveMediaObject(moniker("start_button"), true);
	Core::RemoveMediaObject(moniker("quit_button"), true);
	Core::RemoveMediaObject(moniker("selector_tex"), true);
	Core::RemoveMediaObject(moniker("line_tex"), true);
	Core::RemoveMediaObject(moniker("selbox_corner"), true);
	Core::RemoveMediaObject(moniker("christmas_tree"), true);
	Core::RemoveMediaObject(moniker("snow_corpse"), true);
	Core::RemoveMediaObject(moniker("tos"), true);
	Core::RemoveMediaObject(moniker("qb"), true);
	Core::RemoveMediaObject(moniker("sb"), true);
	singleton<TOW_ObjectManager*>::instance()->Clear(true);
	delete currFrame;
	delete GameScene;
	delete MenuScene;
//	delete map;
	delete ui;
	
	//Core::DebugDump();
	myDevice->Cleanup();
	//delete factory;
	OutputDebugString(Core::ReadLog(-1));
	lua_tow_close();
	char exitbuffer[256];
	sprintf(exitbuffer, "%s\n%i", DN_PROTOCOL_EXIT, net.GetId());
	net.SendData(exitbuffer, strlen(exitbuffer)+1);
	net.Stop();
	net.Terminate();
	Core::Function_Exit();
	return 0;
}

RESULT TimesOfWar_Mod::PreCache(char* file)
{
	return OK;
}

RESULT TimesOfWar_Mod::Think()
{
	//UserInterface* ui = engine->EUserInterface();
	//if(ui->
	return OK;
}