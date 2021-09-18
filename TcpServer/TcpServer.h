#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <future>

#ifdef  _WIN32
	#include <Winsock2.h>
	#include <ws2tcpip.h>

#else //  *nix
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <pthread.h>
#endif

#include "Socket.h"


class MultiThreadTcpServer
{
public:
	//constructor creates server socket, binds it to specified port and sets 
	MultiThreadTcpServer(std::string ipAddr, std::uint16_t port);
	~MultiThreadTcpServer();
	//server's main loop
	void start();
private:
	std::unique_ptr<sock::Socket> servSock;
	//function to get client's data in ASN1 format
	void serveClient(std::unique_ptr<sock::Socket>&& sock);
};