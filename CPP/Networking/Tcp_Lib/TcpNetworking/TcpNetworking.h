#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <WS2tcpip.h>

#define TCPNETWORKING_API __declspec(dllexport)

class TCPNETWORKING_API TcpListener;
class TCPNETWORKING_API TcpClient;
struct TCPNETWORKING_API NetworkNode;
struct TCPNETWORKING_API ServerMessage;