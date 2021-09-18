#include "TcpClient.h"



//constructor connects to specified IP and port
TcpClient::TcpClient(std::string ipAddr, std::uint16_t port)
{
	clientSock = std::make_unique<sock::TcpSocket>();
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	int isValidIp;
#ifdef _WIN32
	isValidIp = inet_pton(AF_INET, &ipAddr[0], &addr.sin_addr.s_addr);
#else
	isValidIp = inet_aton(ipAddr.c_str(), &addr.sin_addr);
#endif
	if (!isValidIp)
		throw std::invalid_argument("IP is not valid");

	std::cout << "Connecting...\n";

	connect(clientSock.get(), addr);

	std::cout << "Connected to server successfully\n";
}
void TcpClient::sendPacket()
{
	std::cout << "Enter message to send to server\n";

	std::string message;
	std::getline(std::cin, message);

	std::vector<char> data;
	data.reserve(message.length());
	std::copy(message.begin(), message.end(), std::back_inserter(data));

	sendAsn1Packet(clientSock.get(), std::move(data));

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(2000ms); //wait for message to be received by server

	std::cout << "Message sent!\n";
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
		TcpClient client("192.168.0.137", 1234);
		client.sendPacket();

#ifdef _WIN32
		WSACleanup();
#endif
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
#ifdef _WIN32
		std::cout << "	" <<  WSAGetLastError();
		WSACleanup();
#else
		std::cout << "	" << errno;
#endif
	}

}
