#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "log_stream.h"
#include "run_server.h"

#include "_socket.h"

int main(int argc, char* argv[])
{
	
	_log l("serverlog.txt");
	l.set_debug_level(2);
	log_stream<1> ls(l);
	WORD version;
	WSADATA wsaData;
	
 
	version = MAKEWORD( 2, 2 );
	WSAStartup(version, &wsaData);
	ls << "Command line was: ";
	for(int i = 0; i < argc; ++i)
	{
		ls << argv[i] << ' ';
	}
	ls << '\n';
	
	run_server(l);
	return 0;
}



