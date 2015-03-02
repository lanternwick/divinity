#ifndef _TIMESOFWAR_MOD_H
#define _TIMESOFWAR_MOD_H

#pragma warning (disable:4231)

//#include <iostream.h>
#include <utility>

#include "..\DivCore\log_stream.h"

#include "..\DivUI\KeyConstants.h"
//#include "..\DivUserInterface\UIConsoleWnd.h"
//#include "..\DivUserInterface\UIButton.h"
#include "..\console\console.h"

#include "..\Common\IMod.h"
//#include "..\DivEngine\DivEngine.h"

#include "..\DivGraph\StaticMesh.h"

#include "Terrain_Tile.h"
#include "TOW_HeightMap.h"
#include "TOW_TextureMap.h"
#include "TOW_Map.h"

#include "TOW_Player.h"
#include "Console\TOW_Console_Functors.h"

#include "TOW_ObjectManager.h"
#include "TOW_TemplateManager.h"

#include "TOW_GameUnit.h"
#include "TOW_GameObjectTemplate.h"
#include "TOW_World.h"
//#include "moving_state.h"
#include "TOW_Selector.h"
#include "TOW_SelectIndicator.h"
#include "TOW_SelectBox.h"
#include "..\DivGraph\Line3D.h"
#include "..\DivUI\UICommandMap.h"
#include "TOW_PathingMap.h"

//#include "UIGameStartButton.h"
//#include "UIGameQuitButton.h"

#include "lua_glue.h"

#include "PostOffice.h"
#include "tow_networking.h"

#include "TOW_InputHandler.h"

#define GAME 0
#define MENU 1

#include "..\Common\profiler.h"

class TimesOfWar_Mod : public IMod
{
public:
	TimesOfWar_Mod();
	~TimesOfWar_Mod();
	
	INT Init(IDevice* device, char* cmdLine);
	INT Run(IDevice* device, char* cmdLine);
	INT Cleanup();

	//CScene* currScene, * GameScene, * MenuScene;
	
	//std::string console_prompt;



	
	//UIWindow* active_window;
	
	TOW_Selector* m_current_sel;
	TOW_SelectBox* m_sel_box;
	TOW_PathingMap* m_p_map;
	VECTOR m_cam_dir;
	TOW_Player* m_player;
	int m_mode;
protected:
	tow_networking net;
	dn_network_protocol protocol;
	dn_session session;
	dn_game game;
	TOW_ObjectManager* m_obj_man;
	IDevice* m_device;
	IGraphObjFactory* m_factory;
	UserInterface* m_ui;
	console tow_console;
	std::vector<TOW_Functor*> m_console_functors;
	bool m_running;
	TOW_InputHandler m_input;
	UICommandMap m_command_map;
	TOW_Map* m_map;
	TOW_World* m_world;
	CCamera m_camera;
	TOW_CameraController m_camera_cont;
	

};

#include "TOW_InputHandler.h"

#endif
