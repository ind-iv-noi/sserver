// client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#define PORT "8800"
WSAData wsadata;
SOCKET connect_socket=INVALID_SOCKET;
int iResult;
const int BUFF_LEN = 512;
int main(){
	addrinfo *result = NULL, *ptr = NULL, hints;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iResult != 0) {
		std::cout << "Startup error";
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo("localhost", PORT, &hints, &result);
	if (iResult != 0) {
		WSACleanup();
		return 1;
	}
	ptr = result;
	connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (connect_socket == INVALID_SOCKET) {
		std::cout << "Sock error\n";
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	iResult = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(connect_socket);
		connect_socket = INVALID_SOCKET;
	}
	freeaddrinfo(result);
	if (connect_socket == INVALID_SOCKET) {
		std::cout << "Connection Error\n";
		WSACleanup();
		return 1;
	}
	char buff[BUFF_LEN],*send_buff=(char *)"Test abc";
	iResult = send(connect_socket, send_buff, strlen(send_buff), 0);
	if (iResult == SOCKET_ERROR) {
		std::cout << "Failed to send\n";
		closesocket(connect_socket);
		WSACleanup();
		return 1;
	}
	iResult = recv(connect_socket, buff, BUFF_LEN, 0);
	std::cout << buff << '\n';
	closesocket(connect_socket);
	WSACleanup();

	return 0;
}
