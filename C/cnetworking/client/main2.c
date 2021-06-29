#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <stdatomic.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

_Atomic char running = 0;

DWORD WINAPI receiveThread(LPVOID lpParam) {
    SOCKET client = *(SOCKET*)lpParam;

    char recvbuf[DEFAULT_BUFLEN];
    int iResult;

    do {
        iResult = recv(client, recvbuf, DEFAULT_BUFLEN, 0);
        recvbuf[iResult] = '\0';
        if ( iResult > 0 ) {
            printf("Bytes received (%d): %s\n", iResult, recvbuf);
        }
        else if ( iResult == 0 ) {
            printf("Connection closed\n");
            running = 0;
            break;
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
        }
    } while (running && iResult > 0);

    return 0;
}

int main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(27015);

        // Create a SOCKET for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Connect to server.
    iResult = connect(ConnectSocket, (struct sockaddr *)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    running = !0;

    // start receive thread
    DWORD thrdId;
    HANDLE recvThread = CreateThread(
        NULL,
        0,
        receiveThread,
        &ConnectSocket, // parameter
        0,
        &thrdId
    );

    if (recvThread) {
        printf("Receive thread started with thread ID: %d\n", thrdId);
    }
    else {
        printf("Receive thread failed: %d\n", GetLastError());
    }

    // send loop
    while (running) {
        scanf("%s", recvbuf);
        recvbuflen = strlen(recvbuf);

        iResult = send(ConnectSocket, recvbuf, recvbuflen, 0);

        if (iResult != recvbuflen) {
            break;
        }
        else if (!memcmp(recvbuf, "/quit", 5)) {
            running = 0;
            break;
        }
    }

    if (CloseHandle(recvThread)) {
        printf("Receive thread closed successfully.\n");
    }

    // shutdown the connection
    iResult = shutdown(ConnectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}