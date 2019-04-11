// test-1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include<vector>
#include<thread>

#pragma comment(lib, "Ws2_32.lib")

#define PORT "8800"
#define DEFAULT_BUFLEN 1024

WSADATA wsaData;
void work_with_client(SOCKET client_socket) {
	
	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	char startmessage[DEFAULT_BUFLEN] = "te salut\n\0";
	send(client_socket, startmessage,DEFAULT_BUFLEN, 0);
	send(client_socket, startmessage, DEFAULT_BUFLEN, 0);
	do{
		memset(recvbuf, 0, sizeof(recvbuf));

		iResult = recv(client_socket, recvbuf, DEFAULT_BUFLEN, 0);

		if (iResult > 0) {
			iSendResult = send(client_socket, recvbuf, DEFAULT_BUFLEN, 0);
			printf("am intrat cu succes\n");
			if (iSendResult == SOCKET_ERROR) {
				printf("no data sent,closing connection\n");
				closesocket(client_socket);
				return;
			}
		}
		else if(iResult==0){
			printf("closing connection\n");
			closesocket(client_socket);
			return;
		}
		else{
			printf("no data received\n");
			closesocket(client_socket);
			return;
		}
	} while (iResult > 0);
	closesocket(client_socket);
}
int main()
{
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	//info_about_server
	addrinfo hints, *result = NULL;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	iResult = getaddrinfo(NULL, PORT, &hints, &result);
	if (iResult != 0) {
		printf("ERROR ERROR ERROR");
		WSACleanup();
		return 1;
	}

	SOCKET listen_socket = INVALID_SOCKET;
	listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_socket == INVALID_SOCKET) {
		printf("eroare la socket boss");
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	//setup socket
	iResult = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult ==SOCKET_ERROR){
		printf("nu a mers bindul,mai incercam");
		closesocket(listen_socket);
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);
    //listen
	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
		printf("ascultam data viitoare,nu merge urechea");
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}

	//get clients;
	SOCKET client_socket=INVALID_SOCKET;
	std::vector <std::thread> threads;
	while (client_socket = accept(listen_socket, NULL, NULL)) {
		threads.push_back(std::thread(work_with_client,client_socket));
		
	}
	for (auto& i : threads) {
		i.join();
	}
    //error
	if (client_socket == INVALID_SOCKET) {
		printf("nu mi place de tine hai pa");
		closesocket(listen_socket);
		WSACleanup();
		return 1;
	}


	system("PAUSE");
	return 0;
}

