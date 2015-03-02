#ifndef _SOCKET_H
#define _SOCKET_H

#include <winsock2.h>
#include <string>

class _socket
{
public:
	_socket();
	_socket(const _socket& s);
	~_socket();

	int init(int port, int type=SOCK_STREAM);
	int close();

	int connect(const std::string& addr);
	int connect_ip(const std::string& ip);

	int listen();
	_socket accept(unsigned long* addr=NULL);

	int send(const char* buf, int len);
	int recv(char* buf, int len, int flags);

	int valid();
private:
	SOCKET m_s;
};

#endif
