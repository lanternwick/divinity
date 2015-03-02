#include "..\Common\custmoniker.h"
#include "..\Common\profiler.h"
#include "update_block.h"
#include "..\DivCore\Core.h"
#include "..\Common\singleton.h"
#include "..\DivUI\UserInterface.h"
#include "TOW_CameraController.h"
#include "TOW_Map.h"

update_block::update_block()
{

}

update_block::~update_block()
{

}

int update_block::step()
{
	UserInterface* ui;
	TOW_CameraController* cam_cont;
	TOW_Map* map;
	cam_cont = singleton<TOW_CameraController*>::instance();
	map = singleton<TOW_Map*>::instance();
	ui = singleton<UserInterface*>::instance();

	if(cam_cont)
	{
		cam_cont->Think();
	}
	if(map)
	{
		map->Think();
	}
	if(ui)
	{
		ui->Think();
	}
	
	return 0;
}