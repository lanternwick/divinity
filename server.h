#ifndef SERVER_H
#define SERVER_H

#pragma warning(disable:4786)

#include "svr_data.h"
#include "console.h"
#include "log_stream.h"

#include <vector>

class server
{
public:
	server(_log& l);
	~server();

	int config(const std::string& cfg);
	int run_config_mode();

	int run();
private:
	server();
	_socket m_sock;
	std::vector<svr_player> m_players;
	console m_console;
	volatile bool m_running;
	_log* m_log;
	console m_con;
	std::vector<console_functor*> m_con_f;
	// functions
	int process_buffer(svr_player& player, char* buffer, int buffer_len);
	int process_text_buffer(svr_player& player, char* buffer, int buffer_len);

};

#endif
