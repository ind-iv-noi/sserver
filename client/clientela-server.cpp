// clientela-server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define PORT "8800"
#define IP_TO_CONNECT "IP DE SEF"
#define BUFLEN 2048
void sendmsg(SOCKET &serv) {
	char receiver[BUFLEN+5];
	char message[BUFLEN];
	char buf[BUFLEN];
	//sends the name first,message second
	// buf[0]=N-receiver name
	//buf[0]=S-message
	//1 exista 0 nu exista
	printf("Please enter the receiver of the message:\n");
	int iresult;
	buf[0] = 1;
	do {
		if (buf[0] == '0')
			printf("Receiver doesn't exist.Please enter a valid one:\n");
		scanf_s("%s", receiver+1, BUFLEN);
		receiver[0] = 'N';
		iresult = send(serv, receiver, BUFLEN, 0);
		iresult = recv(serv, buf, BUFLEN, 0);
	} while (buf[0] == '0');
	printf("Enter the messages.When you wish to stop,send an empty message:");
	scanf_s(" ");
	do{
		fgets(message + 1, BUFLEN, stdin);
		if (message[1] == '\n')
			message[0] = '~';
		else
			message[0] = 'S';
		iresult = send(serv, message, BUFLEN, 0);
//		iresult = recv(serv, message, BUFLEN, 0);
	} while (message[0] != '~');
	return;
}
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
	int typeofact;
	scanf_s("%d", &typeofact);
	while (typeofact > 2 || typeofact < 1) {
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
	iresult = getaddrinfo("koob.dlinkddns.com", PORT, &hints, &result);
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
	char sign[BUFLEN]="MUIE TALIU 123";
	iresult=send(consocket, sign, BUFLEN, 0);
	login(consocket);
	int typeofaction=0;
	while (typeofaction!=3) {
		printf("Please state what you want to do:\n");
		printf("If you want to send a message,type 1\n");
		printf("If you want to check what message you received,type 2\n");
		printf("If you want to stop,type 3\n");
		scanf_s("%d", &typeofaction);
		if (typeofaction < 1 || typeofaction>3) {
			do {
				printf("Please enter a valid number:\n");
				scanf_s("%d", &typeofaction);
			} while (typeofaction < 1 || typeofaction>3);
		}
		if (typeofaction == 1) {
			sendmsg(consocket);
		}
		else {
			break;
		}
	}
	closesocket(consocket);
	WSACleanup();
	system("PAUSE");
	return 0;
}

