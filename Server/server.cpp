#include <windows.networking.sockets.h>
#include <iostream>
#include "../Client/Packet.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char * argv[])
{
	//starts Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return -1;

	//create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ServerSocket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	//binds socket to address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000);
	if (bind(ServerSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(ServerSocket);
		WSACleanup();
		return -1;
	}

	std::cout << "Waiting for client packet" << std::endl;

	//accepts a connection from a client

	while (1) {
		char RxBuffer[128] = {};				//buffer for receiving data
		sockaddr_in CltAddr;					//client Address for sending responses
		int len = sizeof(struct sockaddr_in);	//length parameter for the recvfrom function call

		//add the UDP receive here
		//receive packet from client
		int bytesReceived = recvfrom(ServerSocket, RxBuffer, sizeof(RxBuffer), 0,
		                             (sockaddr*)&CltAddr, &len);
		
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cout << "Error receiving packet: " << WSAGetLastError() << std::endl;
			continue;
		}
		
		std::cout << "\n--- Received " << bytesReceived << " bytes ---" << std::endl;
			
		Packet RxPkt(RxBuffer);

		RxPkt.Display(std::cout);
	}

	closesocket(ServerSocket);	    //closes server socket	
	WSACleanup();					//frees Winsock resources

	return 1;
}