#include "clientSocket.h"
using namespace web_rpc::socket;

#include<winsock2.h>
#include<iostream>
#include<string>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

Client::Client()
{
    
}
bool Client::initSocket()
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return false;
    }
}
void Client::CleanSocket(){
    WSACleanup();
}
Client::~Client()
{
    closesocket(clientSocket);
}

bool Client::connect(const string &ip, int port)
{
    clientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sock_in;
    sock_in.sin_family = AF_INET;
    sock_in.sin_port = htons(port);
    sock_in.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    if (::connect(clientSocket, (sockaddr *)&sock_in, sizeof(sock_in)) == SOCKET_ERROR)
    {
        cout << "Connect error" << endl;
        return 0;
    }
    return 1;
}