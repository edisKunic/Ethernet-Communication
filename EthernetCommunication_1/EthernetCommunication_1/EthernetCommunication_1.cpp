#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib ,"ws2_32.lib")


int main()
{
    std::cout << "Recive data terminal:"<<std::endl;
	int port = 54000;
	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);

	if (wsOk!=0)
	{
		std::cerr << "Cant Initialize winsock!" << std::endl;
		return 0;
	}

	//Create a socket
	SOCKET listeing = socket(AF_INET, SOCK_STREAM, 0);
	if (listeing == INVALID_SOCKET) {
		std::cerr << "Cant Create SOCKET!" << std::endl;
		return 0;
	}

	//attach address and port to socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listeing, (sockaddr*)& hint, sizeof(hint));

	//Listeing 
	listen(listeing, SOMAXCONN);

	//wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listeing, (sockaddr*)& client, &clientSize);
	if (clientSocket==INVALID_SOCKET)
	{
		std::cerr << "Cant Create clientSocket!" << std::endl;
		return 0;
	}

	char host[NI_MAXHOST];
	char service[NI_MAXHOST];
	ZeroMemory(host, NI_MAXHOST); // Fills a block of memory with zeros.
	ZeroMemory(service, NI_MAXHOST);// Fills a block of memory with zeros.

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0)==0)
	{
		std::cout << host << "Connected on port "  << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << "Connected on port " << ntohs(client.sin_port) << std::endl;
	}
	char buffer[4096];
	while (true)
	{
		ZeroMemory(buffer,4096);

		int bytesRecived = recv(clientSocket, buffer, 4096, 0);
		if (bytesRecived==SOCKET_ERROR)
		{
			std::cerr << "Error in reciving bytes! break;" << std::endl;
			break;
		}
		if (bytesRecived==0)
		{
			std::cout << "Client disconected." << std::endl;
			break;
		}if (buffer[0]=='e'&& buffer[1] == 'n' && buffer[2] == 'd') {
			std::cout << "End recived close" << std::endl;
			break;
		}
		std::cout << buffer << std::endl;
	}
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}

