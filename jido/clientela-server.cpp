// clientela-server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define PORT "8800"
#define IP_TO_CONNECT //"your ip"
#define BUFLEN 1024

void login(SOCKET &serv) {
	char user[BUFLEN];
	char pass[BUFLEN];
	char buf[2*BUFLEN];
	//buf[0]-info about login
	//L - login, R - register
	//\0 between user and pass
	//1-login,2-register
	printf("Please register or login if you already have an account\n");
	printf("If you want to login,type 1\n");
	printf("If you want to register a new account,type 2\n");
	loginstart:
	int typeofact;
	scanf_s("%d", &typeofact);
	while (typeofact > 2 && typeofact < 1) {
		printf("Please enter a valid number\n");
		scanf_s("%d", &typeofact);
	}
	int iresult=0;
	memset(user, 0, BUFLEN);
	memset(pass, 0, BUFLEN);
	printf("Please enter your user and password.No spaces allowed in the name or in the password\n");
	do {
		if (iresult > 1) {
			int error = atoi(buf);
			if(error==-1 && typeofact==2)
			    printf("Unsuccesful register.Username not available.Please enter what you wish to do again.\n");
			else if(typeofact==1 && error==-1)
				printf("Unsuccesful login.Please enter what you wish to do again.\n");
			printf("If you want to login,type 1\n");
			printf("If you want to register a new account,type 2\n");
			scanf_s("%d", &typeofact);
		}
		printf("User:\n");
		scanf_s(" %s", user,BUFLEN);
		printf("Password:\n");
		scanf_s(" %s", pass,BUFLEN);
		memset(buf, 0, sizeof(buf));
		if (typeofact == 1)
			buf[0] = 'L';
		else
			buf[0] = 'R';
		strcpy_s(buf + 1,BUFLEN, user);
		strcpy_s(buf + strlen(buf)+1,BUFLEN, pass);
		iresult = send(serv, buf, BUFLEN, 0);
		iresult = recv(serv, buf, BUFLEN, 0);
	} while (iresult > 1);
	printf("Login Succesful!");
}

int main()
{
	WSADATA wsadata;
	int iresult;

	iresult=WSAStartup(MAKEWORD(2,2),&wsadata);
	if (iresult != 0) {
		printf("ERROR ERROR ERROR STARTUP ERROR");
		WSACleanup();
		system("PAUSE");
		return 1;
	}
	addrinfo *result = NULL, hints,*ptr=NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iresult = getaddrinfo(IP_TO_CONNECT, PORT, &hints, &result);
	if (iresult != 0) {
		printf("error gettaddrinfo");
		WSACleanup();
		system("PAUSE");
		return 1;
	}
	SOCKET consocket = INVALID_SOCKET;
	ptr = result;
	consocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (consocket == INVALID_SOCKET) {
		printf("invalid socket");
		freeaddrinfo(result);
		WSACleanup();
		system("PAUSE");
		return 1;
	}
	iresult = connect(consocket, ptr->ai_addr,(int) ptr->ai_addrlen);
	if (iresult == SOCKET_ERROR) {
		closesocket(consocket);
		printf("imposible connection,try harder");
		WSACleanup();
		system("PAUSE");
		return 1;
	}
	freeaddrinfo(result);
	

	//seinding data
	char sendbuf[ BUFLEN ]= "THIS IS A TEST,BREAD";
	char recvbuf[ BUFLEN ];
	int recvbuflen = BUFLEN;
	char sign[BUFLEN]=//"your signature";
	iresult=send(consocket, sign, BUFLEN, 0);
	//buf[0]-info about login
	//L - login, R - register
	//\0 between user and pass
	login(consocket);
	/*iresult = send(consocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iresult == SOCKET_ERROR) {
		printf("sending error\n");
		closesocket(consocket);
		WSACleanup();
		return 1;
	}

	iresult = shutdown(consocket, SD_SEND);
	if (iresult == SOCKET_ERROR) {
		printf("shutdown fail\n");
		closesocket(consocket);
		WSACleanup();
		return 1;
	}
	do {
		iresult = recv(consocket, recvbuf, recvbuflen,0);
		if(iresult)
		  printf("%s\n", recvbuf);
	} while (iresult > 0);*/
	closesocket(consocket);
	WSACleanup();
	system("PAUSE");
	return 0;
}

