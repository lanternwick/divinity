#ifndef CLIENT_H
#define CLIENT_H

#pragma warning(disable:4786)

#include "_socket.h"
#include "IDevice.h"
#include "IInputDevice.h"
#include "UserInterface.h"
#include "console.h"
#include "UICursor.h"
#include "TOW_Map.h"
#include "CCamera.h"

class client
{
public:
	client();
	~client();

	int init(const std::string& config, HWND hWnd);
	int step();
	int term();

private:
	_socket m_s;
	std::string m_config;
	IDevice* m_device;
	UserInterface m_ui;
	console m_con;
	TOW_TextureMap m_tm;
	TOW_HeightMap m_hm;
	TOW_Map m_map;
	CCamera m_cam;
	// functions
	int render();
	int update_world();
	int do_net();
	int do_input();
};

#endif
