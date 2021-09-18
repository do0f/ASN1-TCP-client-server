#include "TcpServer.h"


//constructor creates server socket, binds it to specified port and sets 
MultiThreadTcpServer::MultiThreadTcpServer(std::string ipAddr, std::uint16_t port)
{
	servSock = std::make_unique<sock::TcpSocket>();
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	int isValidIp;
#ifdef _WIN32
	isValidIp = inet_pton(AF_INET, &ipAddr[0], &addr.sin_addr.s_addr);
#else
	isValidIp = inet_aton(ipAddr.c_str(), &addr.sin_addr.s_addr);
#endif
	if (!isValidIp)
		throw std::invalid_argument("IP is not valid");

	bind(servSock.get(), addr);

	std::cout << "Multithreaded server is up.\n";
}
MultiThreadTcpServer::~MultiThreadTcpServer() = default;

//server's main loop
void MultiThreadTcpServer::start()
{
	listen(servSock.get(), SOMAXCONN);
	std::cout << "Started listening...\n";

	while (1)
	{
		auto clientSock = accept(servSock.get());
		std::cout << "Accepted client!\n";
		//asynchronously get clients data in another thread
		auto future = std::async(std::launch::async,
			&MultiThreadTcpServer::serveClient, this, std::move(clientSock));
	}
}

//function to get client's data in ASN1 format
void MultiThreadTcpServer::serveClient(std::unique_ptr<sock::Socket> && sock)
{
	std::cout << "Receiving packet...\n";
	auto packet = sock::recvAsn1Packet(sock.get());
	std::cout << "Packet received! Data is \"";
	std::for_each(packet.cbegin(), packet.cend(), [](char c) {std::cout << c; });
	std::cout << "\"\n";
}


int main()
{
	try {
#ifdef _WIN32
		WSAData wsaData;
		auto returnCode = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (returnCode != 0)
			throw std::runtime_error("WSAStartup error " + std::to_string(returnCode));
#endif
		//construct server with entered IP and port
		MultiThreadTcpServer serv("192.168.0.137", 1234);
		serv.start();

#ifdef _WIN32
		WSACleanup();
#endif
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";

#ifdef _WIN32
		std::cout << "	" <<  WSAGetLastError();
		WSACleanup();
#else
		std::cout << "	" << errno;
#endif
	}

}
