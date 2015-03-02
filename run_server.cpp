#include "server.h"
#include "log_stream.h"


int run_server(_log& l)
{
	// load configuration
	// set up environment
	// open connection
	// listen
	// on connect
	//   authenticate player
	//   add player
	//   send player map
	//   player requests full update
	//   continue

	// normal loop
	// on data
	//   check type { input, full update, chat, whisper }
	//   validate input
	//   send response to all players if needed
	//   continue

	// on disconnect
	//   send discon to all players
	//   remove player data from server
	//   continue
	server s(l);
	s.run_config_mode();
	s.run();
	
	return 0;
}