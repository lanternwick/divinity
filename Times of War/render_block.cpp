#pragma warning(disable:4786)
#include "render_block.h"
#include <fstream>
#include "..\DivCore\Core.h"
#include "..\DivGraph\IDevice.h"
#include "..\DivUI\UserInterface.h"
#include "TOW_Map.h"
#include "..\Common\singleton.h"
#include "TOW_CameraController.h"
#include "..\Common\profiler.h"


render_block::render_block()
{
}

render_block::~render_block()
{
}

int render_block::step()
{
	IDevice* device;
	UserInterface* ui;
	TOW_CameraController* cam_cont;
	TOW_Map* map;
	//m_mutex.lock();
	
	device = singleton<IDevice*>::instance();
	ui = singleton<UserInterface*>::instance();
	map = singleton<TOW_Map*>::instance();
	cam_cont = singleton<TOW_CameraController*>::instance();

	device->Clear(0xff0000ff);
	device->BeginScene();
	if(cam_cont)
	{
		cam_cont->Render();
	}
	if(map)
	{
		map->Render();
	}
	if(ui)
	{
		ui->Render();
	}
	
	device->EndScene();
	device->Flip();
	
	return 0;
}
