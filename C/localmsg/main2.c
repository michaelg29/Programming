#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUFLEN 1024
#define PORT 5500
#define ADDRESS "127.0.0.1" // aka "localhost"

void cleanup(SOCKET listener)
{
    if (listener)
    {
        closesocket(listener);
    }
    WSACleanup();
}

int main()
{
    printf("Hello, world!\n");

    int res, sendRes;
    int running;
    WSADATA wsaData;
    SOCKET listener, client;
    struct sockaddr_in address, clientAddr;
    char recvbuf[BUFLEN];
    char *inputFileContents;
    int inputFileLength;

    // initialization
    res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res)
    {
        printf("Startup failed: %d\n", res);
        return 1;
    }

    // setup server
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == INVALID_SOCKET)
    {
        printf("Error with construction: %d\n", WSAGetLastError());
        cleanup(0);
        return 1;
    }

    // bind server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ADDRESS);
    address.sin_port = htons(PORT);
    res = bind(listener, (struct sockaddr *)&address, sizeof(address));
    if (res == SOCKET_ERROR)
    {
        printf("Bind failed: %d\n", WSAGetLastError());
        cleanup(listener);
        return 1;
    }

    // set as listener
    res = listen(listener, SOMAXCONN);
    if (res == SOCKET_ERROR)
    {
        printf("Listen failed: %d\n", WSAGetLastError());
        cleanup(listener);
        return 1;
    }

    printf("Loading file\n");

    // load input file
    FILE *fp = fopen("input.html", "r");
    if (!fp) {
        printf("Could not load input form\n");
        cleanup(listener);
        return 1;
    }
    // get length
    // move cursor to the end
    fseek(fp, 0L, SEEK_END);
    // get remaining length
    inputFileLength = ftell(fp);
    // return to original position
    fseek(fp, 0, SEEK_SET);
    // read
    inputFileContents = malloc(inputFileLength + 1);
    fread(inputFileContents, inputFileLength, 1, fp);
    inputFileContents[inputFileLength] = 0;

    // done setting up
    printf("Accepting on %s:%d\n", ADDRESS, PORT);
    running = 1;

    while (running)
    {
        // accept client
        int clientAddrLen;
        client = accept(listener, NULL, NULL);
        if (client == INVALID_SOCKET)
        {
            printf("Could not accept: %d\n", WSAGetLastError());
            cleanup(listener);
            return 1;
        }

        // get client info
        getpeername(client, (struct sockaddr *)&clientAddr, &clientAddrLen);
        printf("Client connected at %s:%d\n", inet_ntoa(address.sin_addr));

        // receive
        res = recv(client, recvbuf, BUFLEN, 0);
        if (res > 0)
        {
            // print message
            recvbuf[res] = '\0';
            //printf("%s\n", recvbuf);
            
            // test if GET command
            if (!memcmp(recvbuf, "GET", 3 * sizeof(char)))
            {
                printf("Receive GET\n");
                // return input file
                printf("Requested file\n");
                sendRes = send(client, inputFileContents, inputFileLength, 0);
                if (sendRes == SOCKET_ERROR)
                {
                    printf("Send failed: %d\n", WSAGetLastError());
                }
            }
            // test if POST command
            else if (!memcmp(recvbuf, "POST", 4 * sizeof(char)))
            {
                printf("Receive POST\n");
                // send file back
                sendRes = send(client, inputFileContents, inputFileLength, 0);
                if (sendRes == SOCKET_ERROR)
                {
                    printf("Send failed: %d\n", WSAGetLastError());
                }

                // get message
                // find new line
                int i = res - 1;
                for (; i >= 0; i--) {
                    if (recvbuf[i] == '=') {
                        i++;
                        break;
                    }
                }
                // content from cursor onwards contains data
                //printf("Received (%d, %d, %d): \n%s\n", res, i, res - i, recvbuf + i);
                // get length
                int len = 0;
                for (int j = i; j < res; j++) {
                    len++;
                    if (recvbuf[j] == '%') {
                        j += 2;
                    }
                }
                // read characters
                char *msg = malloc(len + 1);
                for (int cursor = 0, j = i; cursor < len; cursor++, j++) {
                    char c = recvbuf[j];
                    if (c == '%')
                    {
                        // get hex val of next two characters
                        msg[cursor] = 0;
                        for (int k = 1; k <= 2; k++) {
                            c = recvbuf[j + k];
                            if (c >= 'A') {
                                c -= 'A' - 10;
                            }
                            else {
                                c -= '0';
                            }
                            msg[cursor] <<= 4;
                            msg[cursor] |= c;
                        }
                        j += 2;
                    }
                    else if (c == '+') {
                        msg[cursor] = ' ';
                    }
                    else {
                        // output character directly
                        msg[cursor] = c;
                    }
                }
                msg[len] = 0; // terminator
                printf("Parsed (%d): %s\n", len, msg);

                // test message
                if (!memcmp(msg, "/quit", 5 * sizeof(char)))
                {
                    printf("Quit message received\n");
                    running = 0; // false
                    free(msg);
                    break;
                }
                else {
                    printf("%s\n", msg);
                }

                free(msg);
            }
        }
        else if (!res)
        {
            printf("Client disconnected\n");
        }
        else
        {
            printf("Receive failed: %d\n", WSAGetLastError());
        }

        // shutdown client
        shutdown(client, SD_BOTH);
        closesocket(client);
        client = INVALID_SOCKET;
    }

    cleanup(listener);
    printf("Shutting down.\nGood night.\n");

    return 0;
}