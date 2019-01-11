#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (49152)

// forward declaration
class TcpListener;

// Callback to data receieved method
typedef void(*MessageReceivedHandler)(TcpListener* listener, int socketId, std::string msg);
// Callback to server event method
typedef void(*ServerEventHandler)(TcpListener* listener, std::string msg);
// Callback to server command method
typedef void(*CommandHandler)(TcpListener* listener, int socket, std::string cmd);

