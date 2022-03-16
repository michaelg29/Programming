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
#define MAX_CLIENTS 5

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

    // INITIALIZATION ===========================
    WSADATA wsaData; // configuration data
    res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res)
    {
        printf("Startup failed: %d\n", res);
        return 1;
    }
    // ==========================================

    // SETUP SERVER =============================

    // construct socket
    SOCKET listener;
    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == INVALID_SOCKET)
    {
        printf("Error with construction: %d\n", WSAGetLastError());
        cleanup(0);
        return 1;
    }

    // setup for multiple connections
    char multiple = !0;
    res = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &multiple, sizeof(multiple));
    if (res < 0)
    {
        printf("Multiple client setup failed: %d\n", WSAGetLastError());
        cleanup(listener);
        return 1;
    }

    // bind to address
    struct sockaddr_in address;
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

    // set as a listener
    res = listen(listener, SOMAXCONN);
    if (res == SOCKET_ERROR)
    {
        printf("Listen failed: %d\n", WSAGetLastError());
        cleanup(listener);
        return 1;
    }

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
    int inputFileLength = ftell(fp);
    // return to original position
    fseek(fp, 0, SEEK_SET);
    // read
    char *inputFileContents = malloc(inputFileLength + 1);
    fread(inputFileContents, inputFileLength, 1, fp);
    inputFileContents[inputFileLength] = 0;

    // ==========================================

    printf("Accepting on %s:%d\n", ADDRESS, PORT);

    // MAIN LOOP ================================

    // variables
    fd_set socketSet;            // set of active clients
    SOCKET clients[MAX_CLIENTS]; // array of clients
    int curNoClients = 0;        // active slots in the array
    SOCKET sd, max_sd;           // placeholders
    struct sockaddr_in clientAddr;
    int clientAddrlen;
    char running = !0; // server state

    char recvbuf[BUFLEN];

    char *welcome = "Welcome to the server :)\n";
    int welcomeLength = strlen(welcome);
    char *full = "Sorry, the server is full :(\n";
    int fullLength = strlen(full);
    char *goodbye = "Goodnight.\n";
    int goodbyeLength = strlen(goodbye);

    // clear client array
    memset(clients, 0, MAX_CLIENTS * sizeof(SOCKET));

    while (running)
    {
        // clear the set
        FD_ZERO(&socketSet);

        // add listener socket
        FD_SET(listener, &socketSet);

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            // socket
            sd = clients[i];

            if (sd > 0)
            {
                // add an active client to the set
                FD_SET(sd, &socketSet);
            }

            if (sd > max_sd)
            {
                max_sd = sd;
            }
        }

        int activity = select(max_sd + 1, &socketSet, NULL, NULL, NULL);
        if (activity < 0)
        {
            continue;
        }

        // determine if listener has activity
        if (FD_ISSET(listener, &socketSet))
        {
            // accept connection
            sd = accept(listener, NULL, NULL);
            if (sd == INVALID_SOCKET)
            {
                printf("Error accepting: %d\n", WSAGetLastError());
            }

            // get client information
            getpeername(sd, (struct sockaddr *)&clientAddr, &clientAddrlen);
            printf("Client connected at %s:%d\n",
                   inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

            // add to array
            if (curNoClients >= MAX_CLIENTS)
            {
                printf("Full\n");

                // send overflow message
                sendRes = send(sd, full, fullLength, 0);
                if (sendRes != fullLength)
                {
                    printf("Error sending: %d\n", WSAGetLastError());
                }

                shutdown(sd, SD_BOTH);
                closesocket(sd);
            }
            else
            {
                // scan through list
                int i;
                for (i = 0; i < MAX_CLIENTS; i++)
                {
                    if (!clients[i])
                    {
                        clients[i] = sd;
                        printf("Added to the list at index %d\n", i);
                        curNoClients++;
                        break;
                    }
                }
            }
        }

        // iterate through clients
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (!clients[i])
            {
                continue;
            }

            sd = clients[i];
            // determine if client has activity
            if (FD_ISSET(sd, &socketSet))
            {
                // get message
                res = recv(sd, recvbuf, BUFLEN, 0);
                if (res > 0)
                {
                    // print message
                    recvbuf[res] = '\0';
                    
                    // test if GET command
                    if (!memcmp(recvbuf, "GET", 3 * sizeof(char)))
                    {
                        // return input file
                        printf("Requested file\n");
                        sendRes = send(sd, inputFileContents, inputFileLength, 0);
                        if (sendRes == SOCKET_ERROR)
                        {
                            printf("Send failed: %d\n", WSAGetLastError());
                            shutdown(sd, SD_BOTH);
                            closesocket(sd);
                            clients[i] = 0;
                            curNoClients--;
                        }
                    }
                    // test if POST command
                    else if (!memcmp(recvbuf, "POST", 4 * sizeof(char)))
                    {
                        // send file back
                        sendRes = send(sd, inputFileContents, inputFileLength, 0);
                        if (sendRes == SOCKET_ERROR)
                        {
                            printf("Send failed: %d\n", WSAGetLastError());
                            shutdown(sd, SD_BOTH);
                            closesocket(sd);
                            clients[i] = 0;
                            curNoClients--;
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
                            running = 0; // false
                            break;
                        }
                    }
                }
                else
                {
                    // close message
                    getpeername(sd, (struct sockaddr *)&clientAddr, &clientAddrlen);
                    printf("Client disconnected at %s:%d\n",
                           inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

                    shutdown(sd, SD_BOTH);
                    closesocket(sd);
                    clients[i] = 0;
                    curNoClients--;
                }
            }
        }
    }

    // ==========================================

    // CLEANUP ==================================

    // disconnect all clients
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i] > 0)
        {
            // active client
            sendRes = send(clients[i], goodbye, goodbyeLength, 0);

            shutdown(clients[i], SD_BOTH);
            closesocket(clients[i]);
            clients[i] = 0;
        }
    }

    // shut down server socket
    closesocket(listener);

    // cleanup WSA
    res = WSACleanup();
    if (res)
    {
        printf("Cleanup failed: %d\n", res);
        return 1;
    }
    // ==========================================

    printf("Shutting down.\nGood night.\n");

    return 0;
}