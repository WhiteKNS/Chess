#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>

#include <QWidget>

//#include "main_window_setup.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


class TCPClient {
    WSADATA wsaData;
    SOCKET ConnectSocket;
    struct addrinfo *result,
                    *ptr,
                    hints;

    std::string sendbuf;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen;

private:
    int send_data();
    int receive_data();

public:
    TCPClient() {
        ConnectSocket = INVALID_SOCKET;
        result = NULL;
        ptr = NULL;
        sendbuf = "this is a test";
        recvbuflen = DEFAULT_BUFLEN;
    }

    int setup(const char *node_name);
    int run_client();
};

#endif // TCPCLIENT_H
