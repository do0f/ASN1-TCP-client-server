#pragma once
#include <iostream>
#include <exception>
#include <vector>
#include <thread>
#include <chrono>

#ifdef  _WIN32
	#include <Winsock2.h>
	#include <ws2tcpip.h>
#else //  *nix
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#include "Socket.h"

class TcpClient
{
public:
	//constructor connects to specified IP and port
	TcpClient(std::string ipAddr, std::uint16_t port);
	void sendPacket();
private:
	std::unique_ptr<sock::Socket> clientSock;
};