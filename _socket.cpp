#include "_socket.h"
#include "string_tokenizer.h"
#include <sstream>

_socket::_socket()
{
	m_s = -1;
}

_socket::_socket(const _socket& s)
{
	m_s = s.m_s;
}

_socket::~_socket()
{
	
}

int _socket::init(int port, int type)
{
	sockaddr_in sa;
	hostent* he;
	char hostname[1024];
	memset(&sa, 0, sizeof(sa));
	gethostname(hostname, 1023);
	he = gethostbyname(hostname);
	if(he == NULL) // gah
	{
		return -1;
	}
	sa.sin_family = he->h_addrtype;
	sa.sin_port = htons(port);
	m_s = socket(AF_INET, type, 0);
	if(m_s < 0)
	{
		return -1;
	}
	if(bind(m_s, (sockaddr*)&sa, sizeof(sa)) < 0)
	{
		closesocket(m_s);
		m_s = -1;
		return -1;
	}
	
	
	return 0;
}

int _socket::close()
{
	closesocket(m_s);
	m_s = -1;
	return 0;
}

int _socket::connect(const std::string& addr)
{
	string_tokenizer<> s(addr, std::string(":"));
	unsigned short port;
	sockaddr_in sa;
	hostent* he;
	

	if(s.size() == 0)
		return -1;
	if(s.size() > 2)
		return -1;
	if(s.size() == 1)
	{
		// no portnum
	}
	else
	{
		std::stringstream ss(s.token(1));
		ss >> port;
	}
	
	he = gethostbyname(s.token(0).c_str());
	if(he == NULL)
	{
		return -1;
	}
	memset(&sa, 0, sizeof(sa));
	memcpy((char*)&sa.sin_addr, he->h_addr, he->h_length);
	sa.sin_family = he->h_addrtype;
	sa.sin_port = htons(port);
	closesocket(m_s);
	if(::connect(m_s, (sockaddr*)&sa, sizeof(sa)) < 0)
	{
		return -1;
	}
	return 0;
}

int _socket::connect_ip(const std::string& ip)
{

	string_tokenizer<> s(ip, std::string(":"));
	unsigned short port;
	sockaddr_in sa;
	hostent* he;
	

	if(s.size() == 0)
		return -1;
	if(s.size() > 2)
		return -1;
	if(s.size() == 1)
	{
		// no portnum
	}
	else
	{
		std::stringstream ss(s.token(1));
		ss >> port;
	}
	unsigned long ipb = htonl(inet_addr(s.token(0).c_str()));
	if(ipb == INADDR_NONE)
	{
		return -1;
	}
	
	
	he = gethostbyaddr((const char*)&ipb, 4, 0);
	if(he == NULL)
	{
		return -1;
	}
	memset(&sa, 0, sizeof(sa));
	memcpy((char*)&sa.sin_addr, he->h_addr, he->h_length);
	sa.sin_family = he->h_addrtype;
	sa.sin_port = htons(port);
	closesocket(m_s);
	if(::connect(m_s, (sockaddr*)&sa, sizeof(sa)) < 0)
	{
		return -1;
	}
	return 0;
}

int _socket::listen()
{
	::listen(m_s, 3);
	return 0;
}

_socket _socket::accept(unsigned long* addr)
{
	_socket n;
	timeval tv = { 0, 0 };
	fd_set read;
	read.fd_array[0] = m_s;
	read.fd_count = 1;
	if(!select(0, &read, NULL, NULL, &tv))
	{
		return n;
	}
	sockaddr_in sa;
	int len = sizeof(sa);
	n.m_s = ::accept(m_s, (sockaddr*)&sa, &len);
	if(n.m_s == INVALID_SOCKET)
	{
		// no connections
		n.m_s = -1;
	}
	*addr = sa.sin_addr.S_un.S_addr;
	return n;
}

int _socket::send(const char* buf, int len)
{
	return ::send(m_s, buf, len, 0);
	
}

int _socket::recv(char* buf, int len, int flags)
{
	timeval tv = { 0, 0 };
	fd_set read;
	read.fd_array[0] = m_s;
	read.fd_count = 1;
	if(!select(0, &read, NULL, NULL, &tv))
	{
		return WSAEWOULDBLOCK;
	}
	return ::recv(m_s, buf, len, flags);
}

int _socket::valid()
{
	return ((m_s == 0xffffffff) ? 0 : 1);
}
