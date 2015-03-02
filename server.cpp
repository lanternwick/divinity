#include "server.h"
#include "gwen.h"
#include "svr_funcs.h"
#include <conio.h>

server::server()
{
	m_log = NULL;
	m_running = false;
}

server::server(_log& l)
{
	m_log = &l;
	m_running = false;
}

server::~server()
{
	for(std::vector<console_functor*>::iterator i = m_con_f.begin(); i != m_con_f.end(); ++i)
	{
		delete (*i);
	}

}

int server::config(const std::string& cfg)
{
	return 0;
}

int server::run_config_mode()
{
	m_con.set_security_level(1);
	m_con_f.push_back(new quit(&m_running));
	m_con.add_function(std::string("function quit()"), m_con_f.back(), 1);

	return 0;
}

#define RANDOM_PORT 12345
#define BUFFER_SIZE 512

int server::run()
{
	int num_read = 0;
	int pb_ret = 0;
	log_stream<1> ls(*m_log);
	log_stream<2> crit(*m_log);
	
	

	m_sock.init(RANDOM_PORT);
	if(!m_sock.valid())
	{
		return -1;
	}

	m_sock.listen();

	ls << "Server listening on port " << RANDOM_PORT << ".\n";

	m_running = true;
	//begin();
	_socket new_conn;
	unsigned long inc_ip;
	std::string in;
	char c;
	while(m_running)
	{
			
		// check for input
		if(_kbhit())
		{
			c= _getche();
			if(c == '\r')
			{
				_putch('\n');
				m_con.exec_command(in);
				in.erase();
				
				c = 0;
			}
			else
			{
				in += c;
			}
		}
		// any new connections?
		new_conn = m_sock.accept(&inc_ip);
		if(new_conn.valid())
		{
			ls << "New connection from " << inet_ntoa(*((in_addr*)&inc_ip)) << ".  Creating new player.\n";
			// new connection
			svr_player np;
			// check max players
			// create new player
			np.conn = new_conn;
			np.data_offset = 0;
			np.data_incomp = false;
			// set player to login mode
			np.level = 0;
			np.ip_addr = inc_ip;
			np.pid = m_players.size()+1;
			np.valid = true;
			m_players.push_back(np);
		}
		
		for(std::vector<svr_player>::iterator i = m_players.begin(); i != m_players.end(); ++i)
		{
			// not a valid player, continue along
			if(!(*i).valid)
			{
				continue;
			}
			
			num_read = (*i).conn.recv((*i).data_buffer, svr_player_data_size, 0);
			if(num_read == 0)
			{
				ls  << (*i).player_name << "(" << inet_ntoa(*((in_addr*)&(*i).ip_addr)) << ") disconnected.\n";
				(*i).valid = false;
				// destruct player here
			}
			else if(num_read == SOCKET_ERROR)
			{
				crit << "SOCK: Some sort of error occurred(" << WSAGetLastError() << ").\n";
				(*i).valid = false;
			}
			else if(num_read == WSAEWOULDBLOCK)
			{
				// nothing waiting for me
			}
			else
			{
				if((*i).data_buffer[0] == 't')
				{
					// text mode buffer
					// no safety net on this one, only for testing
					pb_ret = process_text_buffer((*i), (*i).data_buffer, num_read);
				}
				else
				{
					// binary mode buffer
					pb_ret = process_buffer((*i), (*i).data_buffer, num_read+(*i).data_offset);
				}
				if(pb_ret == -1)
				{
					(*i).data_incomp = true;
					(*i).data_offset += num_read;
					continue;
				}
			}			
		}

	}

	m_sock.close();
	return 0;
}


/*
MID list
0: OK
1: Not OK
2: sending username
3: sending password
4: talk message
5: whisper message
6: Disconnect

*/

int server::process_buffer(svr_player& player, char* buffer, int buffer_len)
{
	svr_header head;
	svr_header response;
	svr_arg_info* args_info;
	int d_pid;
	svr_player* d_player;
	log_stream<1> ls(*m_log);
	log_stream<2> crit(*m_log);
	if(buffer_len < sizeof(head))
	{
		return -1;
	}
	memcpy(&head, buffer, sizeof(head));
	if(buffer_len != head.mlen)
	{
		return -1;
	}
	// reset
	player.data_incomp = false;
	player.data_offset = 0;

	if(head.pid != player.pid) 
	{
		return -1;
	}

	if(head.num_args)
	{
		args_info = (svr_arg_info*)buffer+sizeof(head);
	}

	// do stuff with message
	std::vector<svr_player>::iterator i;
	switch(head.mid)
	{
	case 0:  // OK
		break;
	case 1: // Not OK
		break;
	case 2: // username
		if(head.num_args != 1)
		{
			response.pid = 0;
			response.mid = 1;
			response.num_args = 0;
			response.mnum = head.mnum;
			response.mlen = sizeof(response);
			player.conn.send((char*)&response, sizeof(response));
			break;
		}
		ls << "Player " << player.pid << "(" << player.player_name << ") changed name to ";
		player.player_name = buffer+args_info[0].offset;
		ls << player.player_name << ".\n";
		
		break;
	case 3: // password
		break;
	case 4: // talk
		if(head.num_args != 1)
		{
			response.pid = 0;
			response.mid = 1;
			response.num_args = 0;
			response.mnum = head.mnum;
			response.mlen = sizeof(response);
			player.conn.send((char*)&response, sizeof(response));
			break;
		}
		ls << "***<" << player.player_name << "> " << buffer+args_info[0].offset << '\n';
		for(i = m_players.begin(); i != m_players.end(); ++i)
		{
			(*i).conn.send(buffer, buffer_len);
		}
		break;
	case 5: // whisper
		if(head.num_args != 2)
		{
			response.pid = 0;
			response.mid = 1;
			response.num_args = 0;
			response.mnum = head.mnum;
			response.mlen = sizeof(response);
			player.conn.send((char*)&response, sizeof(response));
			break;
		}
		d_pid = *((int*)buffer_len+args_info[0].offset);
		for(i = m_players.begin(); i != m_players.end(); ++i)
		{
			if((*i).pid == d_pid)
			{
				d_player = &(*i);
				break;
			}
		}
		ls << "WWW<" << player.player_name << "> to <" << d_player->player_name << "> " << buffer+args_info[1].offset << '\n';
		d_player->conn.send(buffer, buffer_len);
		break;
	}


	return 0;
}

int server::process_text_buffer(svr_player& player, char* buffer, int buffer_len)
{
	std::string response;
	// get rid of the 't' that marks this as a text buffer
	// and make it into a string
	++buffer;
	buffer[buffer_len-2 ] = 0;
	string_tokenizer<> s(buffer, " ");
	int d_pid;
	svr_player* d_player;
	log_stream<1> ls(*m_log);
	log_stream<2> crit(*m_log);
	
	// reset
	player.data_incomp = false;
	player.data_offset = 0;

	// do stuff with message
	std::vector<svr_player>::iterator i;
	if(s.token(0) == "OK")
	{
	}
	else if(s.token(0) == "NOK")
	{
	}
	else if(s.token(0) == "USER")
	{
		if(player.level < LEVEL_LOGIN)
		{
			return 0;
		}
		if(s.size() != 2)
		{
			response = "tNOK";
			player.conn.send(response.c_str(), response.length()+1);
			return 0;
		}
		ls << "Player " << player.pid << "(" << player.player_name << ") changed name to ";
		player.player_name = s.token(1);
		ls << player.player_name << ".\n";
		if(player.level == LEVEL_LOGIN)
		{
			player.level = LEVEL_NORM;
		}
	}
	else if(s.token(0) == "PASS")
	{
	}
	else if(s.token(0) == "TALK")
	{
		if(player.level < LEVEL_NORM)
		{
			return 0;
		}
		if(s.size() != 2)
		{
			response = "tNOK";
			player.conn.send(response.c_str(), response.length()+1);
			return 0;
		}
		ls << "***<" << player.player_name << "> " << s.token(1) << '\n';
		for(i = m_players.begin(); i != m_players.end(); ++i)
		{
			(*i).conn.send(buffer, buffer_len);
		}
	}
	else if(s.token(0) == "WHSP")
	{
		if(player.level == 0)
		{
			return 0;
		}
		if(s.size() != 3)
		{
			response = "tNOK";
			player.conn.send(response.c_str(), response.length()+1);
			return 0;
		}
		d_pid = atoi(s.token(1).c_str());
		d_player = NULL;
		for(i = m_players.begin(); i != m_players.end(); ++i)
		{
			if((*i).pid == d_pid)
			{
				d_player = &(*i);
			}
		}
		if(!d_player)
		{
			response = "tNOK";
			player.conn.send(response.c_str(), response.length()+1);
			return 0;
		}
		ls << "WWW<" << player.player_name << "> to <" << d_player->player_name << "> " << s.token(2) << '\n';
		d_player->conn.send(buffer, buffer_len);
	}
	else if(s.token(0) == "QUIT")
	{
		player.conn.close();
		ls  << player.player_name << "(" << inet_ntoa(*((in_addr*)&player.ip_addr)) << ") disconnected.\n";
		player.valid = false;
	}
	else if(s.token(0) == "PROTO")
	{
		if(player.level != LEVEL_LOW)
		{
			// we can't protocol change once we've done it!
			return 0;
		}
		if(s.size() != 2)
		{
			response = "tNOK";
			player.conn.send(response.c_str(), response.length()+1);
			return 0;
		}
		if(s.token(1) != SVR_PROTOCOL_STRING)
		{
			response = "tNOK";
			player.conn.send(response.c_str(), response.length()+1);
			player.conn.close();
			ls  << inet_ntoa(*((in_addr*)&player.ip_addr)) << "disconnected. Bad Protocol!\n";
			player.valid = false;
			return 0;
		}
		response = "tOK";
		player.conn.send(response.c_str(), response.length()+1);
		ls  << inet_ntoa(*((in_addr*)&player.ip_addr)) << " protocol OK.\n";
		player.level = LEVEL_LOGIN;
	}
	else
	{
	}
	
	return 0;
}

