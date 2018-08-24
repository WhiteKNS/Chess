#include "TCPClient.h"
#include "tile.h"
#include "game_info.h"

#include <iostream>
#include <string>
#include <sstream>
#include <thread>

#include <chrono>

#ifdef _DEBUG
#include <assert.h>
#else
void assert(std::string str) {
    std::cout << "something wrong" << std::endl;
}
#endif // _DEBUG

extern void fill_data();

int player_color;
extern int tileNum, tileNum_click, pieceColor;
extern char pieceName;
extern bool send_to_server;

extern Tile *click1;
extern Tile *tile[8][8];

int TCPClient::setup(const char *node_name) {
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        qDebug() << "WSAStartup failed with error: " <<iResult;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(node_name, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        qDebug() << "getaddrinfo failed with error::" << iResult;
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            qDebug() << "socket failed with error:" << WSAGetLastError();
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        qDebug() << "Unable to connect to server!";
        WSACleanup();
        return 1;
    }

    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

    if (iResult == SOCKET_ERROR) {
        qDebug() <<"error " << WSAGetLastError();
    }
    QString qs = QString::fromUtf8(recvbuf);
    std::string color = qs.toLocal8Bit().constData();

    qDebug() << qs;
    if(color == "white")
        player_color = 1;
    else if (color == "black")
        player_color = 0;
    else
        return 1;

    return 0;
}

int TCPClient::receive_data()
{
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

    if (iResult > 0) {
        qDebug() << "received client: " << recvbuf;

        std::istringstream f(recvbuf);
        std::string parces_str;

        int params_count = 0;
        while (std::getline(f, parces_str, ' ')) {
            switch (params_count) {
            case e_game_info::tile_num:
                tileNum = atoi(parces_str.c_str());
                break;
            case e_game_info::piece_name:
                pieceName = parces_str.c_str()[0];
                break;
            case e_game_info::tile_num_click:
                tileNum_click = atoi(parces_str.c_str());
                break;
            case e_game_info::piece_color:
                pieceColor = atoi(parces_str.c_str());
            default:
                assert("wrong data received!");
            }

            ++params_count;
        }


        fill_data();
    }
    else if (iResult == 0) {
       std::cout << "it is no bytes received: %d" << iResult << std::endl;
       return 1;
   }
   else if (iResult == SOCKET_ERROR) {
        printf("recv failed with error: %d\n", WSAGetLastError());
        return SOCKET_ERROR;
   }

   return 0;
}

int TCPClient::send_data()
{
    qDebug() << "send_data client";

    while(!send_to_server)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string send_string = std::to_string(tileNum) + ' ' + pieceName + ' ' + std::to_string(tileNum_click) + ' ' + std::to_string(pieceColor);

    iResult = send(ConnectSocket, send_string.c_str(), size_t(send_string.length()) + 1, 0);

    //if (iResult >= 0) {
    if (iResult == SOCKET_ERROR)
        qDebug() << "recv failed with error:" << WSAGetLastError();

    send_to_server = false;

    return iResult != SOCKET_ERROR;
}

int TCPClient::run_client()
{
    while (true) {
        if (iResult == SOCKET_ERROR) {
            printf("client failed with error: %d\n", WSAGetLastError());
            break;
        }

        if (player_color) {
            qDebug() << "waiting move from white player\n";
            send_data();
            qDebug() << "waiting move from black player\n";
            receive_data();
        } else {
            qDebug() << "waiting move from white 2 player\n";
            receive_data();
            qDebug() << "waiting move from black  2 player\n";
            send_data();
        }

    }

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        qDebug() << "shutdown failed with error: " << WSAGetLastError();
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return iResult;
}
