#ifndef SVR_DATA_H
#define SVR_DATA_H
// server data header file, contains data structures for server

#include <vector>
#include <string>
#include "_socket.h"

#define LEVEL_LOW 0
#define LEVEL_LOGIN 1
#define LEVEL_NORM 2


#define svr_player_data_size 512
#define SVR_PROTOCOL 0
#define SVR_PROTOCOL_STRING "0"


struct svr_player
{
	std::string player_name;
	int pid;
	_socket conn;
	int level;
	unsigned long ip_addr;
	bool valid;
	//std::vector<unit*> units;
	//std::vector<building*> buildings;
	
	bool data_incomp;
	int data_offset;
	char data_buffer[svr_player_data_size];

};

struct svr_header
{
	int pid; // player id
	int mid; // message type
	int mlen; // message len
	unsigned long mnum; // message number
	int num_args; // number of args
};

struct svr_arg_info
{
	short offset;
	short size;
};


#endif
