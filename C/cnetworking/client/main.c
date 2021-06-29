#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

typedef char bool;
#define false (bool)0
#define true !false

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
   void WSAAPI freeaddrinfo( struct addrinfo* );

   int WSAAPI getaddrinfo( const char*, const char*, const struct addrinfo*,
                 struct addrinfo** );

   int WSAAPI getnameinfo( const struct sockaddr*, socklen_t, char*, DWORD,
                char*, DWORD, int );
#ifdef __cplusplus
}
#endif

#pragma comment(lib, "Ws2_32.lib")

#define PORT 27015
#define ADDRESS "127.0.0.1"
#define BUFLEN 512
#define MAX_CLIENTS 30

int main() {
    // VARIABLES =======================
    WSADATA wsaData; // WSA configuration data

    int res, sendRes; // results to WSA function calls
    char buf[BUFLEN]; // message buffer
    int buflen = BUFLEN; // length of the buffer

    struct sockaddr_in address; // server address
    int addrlen;
    SOCKET listener; // listener socket

    char *test = "Test.\n"; // test message for clients

    printf("Hello, world!\n");

    // WINSOCK API (WSA) INITIALIZATION ===================================
    res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res) {
        printf("Startup failed: %d\n", res);
        return 1;
    }

    // SETUP SERVER STRUCTURE =================================
    address.sin_family = AF_UNSPEC;
    address.sin_addr.s_addr = inet_addr(ADDRESS);
    address.sin_port = htons(PORT);

    struct addrinfo hints;
    struct addrinfo *result;
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    res = getaddrinfo(ADDRESS, "27015", &hints, &result);
    if (res) {
        printf("getaddrinfo failed with error: %d\n", res);
        WSACleanup();
        return 1;
    }

    // socket construction
    for (int i = 0; i < 1; i++) {
        //listener = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
        listener = socket(result->ai_family, result->ai_socktype, 
                result->ai_protocol);
        if (listener == INVALID_SOCKET) {
            printf("Error at socket(): %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // connect
        getpeername(listener, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        printf("Connecting to %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        res = connect(listener, (struct sockaddr*)&address, sizeof(address));
        if (!res) {
            break;
        }
        else if (res == SOCKET_ERROR) {
            printf("Could not connect.\n");
            closesocket(listener);
            listener = INVALID_SOCKET;
        }
    }
    

    // MAIN LOOP ===================================

    // receive welcome
    res = recv(listener, buf, buflen, 0);
    if (res > 0) {
        printf("Received (%d): %s", res, buf);
    }
    else if (!res) {
        printf("Connection closed\n");
    }
    else {
        printf("Recv error: %d\n", WSAGetLastError());
        closesocket(listener);
        WSACleanup();
        return 1;
    }

    do {
        // get input
        scanf("%s", buf);
        sendRes = send(listener, buf, strlen(buf), 0);
        if (sendRes == SOCKET_ERROR) {
            printf("Could not send: %d\n", WSAGetLastError());
            closesocket(listener);
            WSACleanup();
            return 1;
        }

        res = recv(listener, buf, buflen, 0);
        if (res > 0) {
            printf("Received (%d): %s", res, buf);
        }
        else if (!res) {
            printf("Connection closed\n");
            break;
        }
        else {
            printf("Recv error: %d\n", WSAGetLastError());
            closesocket(listener);
            WSACleanup();
            return 1;
        }
    } while (!memcmp("/quit", buf, 5 * sizeof(char))); 

    printf("Shutting down.\nGood night.\n");

    shutdown(listener, SD_BOTH);
    closesocket(listener);
    WSACleanup();

    return 0;
}