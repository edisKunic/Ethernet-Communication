#include "pch.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib ,"ws2_32.lib")

int main()
{
    std::cout << "Terminal for sending messages. ( type-> end - for close )\n";

	std::string ipAddress = "127.0.0.1";
	int port = 54000;
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int init_check = WSAStartup(ver, &wsData);

	if (init_check != 0)
	{
		std::cerr << "Cant Initialize winsock!" << std::endl;
		return 0;
	}

	SOCKET ourSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ourSocket == INVALID_SOCKET) { 
		std::cerr << "Cant Create SOCKET!" << std::endl;
		return 0;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);

	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	
	int connection = connect(ourSocket, (sockaddr*)&hint, sizeof(hint));
	if (connection==SOCKET_ERROR)
	{
		std::cerr << "Cant connect to server" << std::endl;
		closesocket(ourSocket);
		WSACleanup();
		return 0;
	}

	char buffer[4096];
	std::string input;
	while (input != "end")
	{
		getline(std::cin, input);
		if (input.size()>0)
		{
			int rez = send(ourSocket, input.c_str(), input.size() + 1, 0);
		}
	}

	closesocket(ourSocket);
	WSACleanup();
	return 0;
}

