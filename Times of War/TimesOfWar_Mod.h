#ifndef _TIMESOFWAR_MOD_H
#define _TIMESOFWAR_MOD_H

#pragma warning (disable:4231)

//#include <iostream.h>
#include <utility>

#include "..\DivCore\log_stream.h"

#include "..\DivUserInterface\KeyConstants.h"
#include "..\DivUserInterface\UIConsoleWnd.h"
#include "..\DivUserInterface\UIButton.h"
#include "Console\console.h"

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
#include "TOW_OutputManager.h"
#include "TOW_GameUnit.h"
#include "TOW_GameObjectTemplate.h"
#include "TOW_World.h"
//#include "moving_state.h"
#include "TOW_Selector.h"
#include "TOW_SelectIndicator.h"
#include "TOW_SelectBox.h"
#include "..\DivGraph\Line3D.h"
#include "..\DivUserInterface\UICommandMap.h"
#include "TOW_PathingMap.h"

#include "UIGameStartButton.h"
#include "UIGameQuitButton.h"

#include "lua_glue.h"

#include "PostOffice.h"
#include "tow_networking.h"

#define GAME 0
#define MENU 1

class TimesOfWar_Mod : public IMod
{
public:
	TimesOfWar_Mod();
	~TimesOfWar_Mod();
	
	INT Init(IDevice* device, char* cmdLine);
	INT Run(IDevice* device, char* cmdLine);
	INT Cleanup();

	RESULT DrawFrame(CFrame* frameToRender);
	RESULT UpdateFrame(CScene* sceneToUpdate);

	RESULT PreCache(char* file);
	RESULT Think();

	IDevice* myDevice;
	CScene* currScene, * GameScene, * MenuScene;
	UserInterface* ui;
	TOW_Map* map;
	float xPos;
	float zPos;
	float yAdd;
	bool running;
	console tow_console;
	std::string console_prompt;

	TOW_OutputManager<> m_guOutputMan;

	TOW_World* world;
	UIWindow* active_window;
	UICommandMap m_commandmap;
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
	


};

#include "TOW_InputHandler.h"

#endif
