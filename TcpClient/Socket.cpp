#include "Socket.h"

namespace sock {
	int getSocketError()
	{
#ifdef _WIN32
		return WSAGetLastError();
#else
		return errno;
#endif
	}

	//abstract class for socket

	Socket::Socket() = default;
	Socket::~Socket() = default;


	TcpSocket::TcpSocket()
	{
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET)
			throw std::runtime_error("Error creating socket");
	}

	//define constructor from SOCKET for win or int for *nix
#ifdef _WIN32
	TcpSocket::TcpSocket(SOCKET rawSocket)
	{
		sock = rawSocket;
	}
#else
	TcpSocket::TcpSocket(int rawSocket)
	{
		sock = rawSocket;
	}
#endif // _WIN32

	TcpSocket::~TcpSocket()
	{
#ifdef _WIN32
		closesocket(sock);
#else
		close(sock);
#endif
	}

	//friend wrapping functions for Socket class
	void bind(Socket* sock, sockaddr_in& addr)
	{
		if (bind(sock->sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
			throw std::runtime_error("bind error " + std::to_string(getSocketError()));
	}
	void listen(Socket* sock, int backlog)
	{
		if (::listen(sock->sock, backlog) == SOCKET_ERROR)
			throw std::runtime_error("listen error " + std::to_string(getSocketError()));
	}
	std::unique_ptr<Socket> accept(Socket* sock)
	{
		auto acceptedSocket = ::accept(sock->sock, nullptr, nullptr);
		if (acceptedSocket == INVALID_SOCKET)
			throw std::runtime_error("accept error " + std::to_string(getSocketError()));

		return std::make_unique<TcpSocket>(acceptedSocket);
	}
	void connect(Socket* sock, sockaddr_in& addr)
	{
		if (::connect(sock->sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
			throw std::runtime_error("connect error " + std::to_string(getSocketError()));
	}
	std::vector<char> recv(Socket* sock, std::size_t size)
	{
		std::vector<char> data(size);
		std::size_t bytesRecv = 0;
		while (bytesRecv != size)
		{
			auto recvRes = ::recv(sock->sock, &data[bytesRecv], size - bytesRecv, 0);
			if (recvRes == SOCKET_ERROR)
				throw std::runtime_error("recv error " + std::to_string(getSocketError()));
			else bytesRecv += recvRes;
		}
		return data;
	}
	void send(Socket* sock, std::vector<char>& data)
	{
		auto size = data.size();
		std::size_t bytesSend = 0;
		while (bytesSend != size)
		{
			auto sendRes = ::send(sock->sock, &data[bytesSend], size - bytesSend, 0);
			if (sendRes == SOCKET_ERROR)
				throw std::runtime_error("send error " + std::to_string(getSocketError()));
			else bytesSend += sendRes;
		}
	}

	//receive data with length encoded in ASN1 format
	std::vector<char> recvAsn1Packet(Socket* sock)
	{
		std::size_t packetLength = asn1::decodeLength(recv, sock);
		//when packet length is got, construct vector for data receiving
		auto packet = recv(sock, packetLength);
		return packet;
	}
	//get data, encode legth in ASN1 format and send it
	void sendAsn1Packet(Socket* sock, std::vector<char>&& data)
	{
		//get vector of length bytes
		auto asn1Packet = asn1::encodeLength(data.size());
		//construct asn1 format message
		std::copy(data.begin(), data.end(), std::back_inserter(asn1Packet));
		send(sock, asn1Packet);
	}
}