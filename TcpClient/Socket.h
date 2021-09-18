#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iterator>

#ifdef  _WIN32
	#include <Winsock2.h>
#else //  *nix
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
    #include <unistd.h>
	//macros for windows compatibility
	#define SOCKET_ERROR -1
	#define INVALID_SOCKET -1
#endif

#include "Asn1.h"

namespace sock {
	int getSocketError();

	//abstract class for socket
	class Socket
	{
	public:
		Socket();
		virtual ~Socket();

		friend void bind(Socket* sock, sockaddr_in& addr);
		friend void listen(Socket* sock, int backlog);
		friend std::unique_ptr<Socket> accept(Socket* sock);
		friend void connect(Socket* sock, sockaddr_in& addr);
		friend std::vector<char> recv(Socket* sock, std::size_t size);
		friend void send(Socket* sock, std::vector<char>& data);
	protected:
#ifdef _WIN32
		SOCKET sock = INVALID_SOCKET;
#else
		int sock = INVALID_SOCKET;
#endif
	};

	//concrete socket class
	class TcpSocket : public Socket
	{
	public:
		TcpSocket();
		//define constructor from SOCKET for win or int for *nix
#ifdef _WIN32
		TcpSocket(SOCKET rawSocket);
#else
		TcpSocket(int rawSocket);
#endif // _WIN32
		virtual ~TcpSocket();
	private:
	};

	//friend wrapping functions for Socket class
	void bind(Socket* sock, sockaddr_in& addr);
	void listen(Socket* sock, int backlog);
	std::unique_ptr<Socket> accept(Socket* sock);
	void connect(Socket* sock, sockaddr_in& addr);
	std::vector<char> recv(Socket* sock, std::size_t size);
	void send(Socket* sock, std::vector<char>& data);

	//receive data with length encoded in ASN1 format
	std::vector<char> recvAsn1Packet(Socket* sock);
	//get data, encode legth in ASN1 format and send it
	void sendAsn1Packet(Socket* sock, std::vector<char>&& data);
}