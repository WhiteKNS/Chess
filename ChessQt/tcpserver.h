#ifndef TCPSERVER_H
#define TCPSERVER_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#pragma once
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

//using namespace std;
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


class TCPServer
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen;

private:

    void receive_data();
    void send_data();

public:
    TCPServer() {
        ListenSocket = INVALID_SOCKET;
        ClientSocket = INVALID_SOCKET;

        recvbuflen = DEFAULT_BUFLEN;
        result = NULL;
    }

    int setup();

    int run_server();
};


#endif // TCPSERVER_H
