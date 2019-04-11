#include "pch.h"
#define PORT "8800"
std::map<std::string, std::string>user_password;
std::ifstream read_file("user.info", std::ios::binary | std::ios::app);
std::ofstream act_users("user.info", std::ios::binary | std::ios::app);
const int BUFF_LEN = 2048;
void create_map() {
	std::string u, p;
	while (read_file >> u >> p >> std::ws)
		user_password[u] = p;
}
bool register_user(std::string user,std::string password){
	if (user_password.find(user) == user_password.end()) {
		user_password[user] = password;
		act_users.seekp(act_users.end);
		act_users << user << " " << password << '\n';
		act_users.flush();
		return 1;
	}
	return 0;
}

int login_user(std::string user, std::string password) {
	if (user_password.find(user) == user_password.end()) 
		return 0;
	if (user_password[user] != password)
		return -1;
	return 1;
}

void func(SOCKET cli) {
	char read_buff[BUFF_LEN],user[BUFF_LEN],pass[BUFF_LEN],send_buff[BUFF_LEN];
	int iResult, iSendResult,i,j;
	iResult = recv(cli, read_buff, BUFF_LEN, 0);
	if (strcmp(read_buff, "eu ma duc la piata si cumpar o rata") != 0) {
		closesocket(cli);
		return;
	}
	bool logged_in = 0;
	do {
		//Trimite inapoi mesajul primit de la cilent
		if (!logged_in) {
			memset(read_buff, 0, sizeof(read_buff));
			memset(send_buff, 0, sizeof(send_buff));
			iResult = recv(cli, read_buff, BUFF_LEN, 0);
			if (iResult > 0) {
				std::cout << "Recieved " << iResult << " bytes\n";
				std::cout << "Message Recieved : " << read_buff << '\n';
				if (!logged_in) {
					memset(user, 0, sizeof(user));
					memset(pass, 0, sizeof(pass));
					for (i = 1; i < BUFF_LEN && read_buff[i] != '\0'; i++)
						user[i - 1] = read_buff[i];
					for (i++, j = 0; i < BUFF_LEN && read_buff[i] != '\0'; i++, j++)
						pass[j] = read_buff[i];
					if (read_buff[0] == 'L')
						if (login_user(user, pass) > 0)
							sprintf_s(send_buff, "1\0"),logged_in=1;
						else
							sprintf_s(send_buff, "-1\0");
					else
						if (register_user(user, pass))
							sprintf_s(send_buff, "1\0"), logged_in = 1;
						else
							sprintf_s(send_buff, "-1\0");
					iSendResult = send(cli, send_buff,strlen(send_buff), 0);
					std::cout << "Sent:" << " " << iSendResult;
				}
				else {

				}


			}
			else
				if (iResult == 0) {
					std::cout << "Conection closed\n";
					closesocket(cli);
					return;
				}
				else {
					std::cout << "Esti prost\n";
					closesocket(cli);
					return;
				}
		}
	} while (iResult > 0);
	closesocket(cli);
}
WSADATA wsaData;
int main()
{
	
	addrinfo *result = NULL, *ptr = NULL, hints;
	SOCKET ListenSocket = INVALID_SOCKET, Client_Socket = INVALID_SOCKET;
	int iResult;
	std::vector<std::thread>threads;
	//System-Check
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	//Server info setup
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(NULL, PORT, &hints, &result);
	if (iResult != 0) {
		WSACleanup();
		return 1;
	}
	//Open Socket
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		return 2;
	}
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 3;
	}
	freeaddrinfo(result);
	//Open client connections
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		WSACleanup();
		return 4;
	}
	create_map();
	std::cout << "am ajuns pana aici\n";
	while (Client_Socket = accept(ListenSocket, NULL, NULL)) {
		std::cout << "Client nou\n";
		threads.push_back(std::thread(func, Client_Socket));
	}
	if (Client_Socket == INVALID_SOCKET) {
		WSACleanup();
		return 5;
	}
	//Wait for all clients
	for (auto & t : threads)
		t.join();
	system("PAUSE");
	return 0;
}

