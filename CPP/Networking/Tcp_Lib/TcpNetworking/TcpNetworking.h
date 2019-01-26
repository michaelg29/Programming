#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <WS2tcpip.h>

//#ifdef TCPNETWORKING_EXPORT
#define TCPNETWORKING_API __declspec(dllexport)
//#else
//#define TCPNETWORKING_API __declspec(dllimport)
//#endif

class TCPNETWORKING_API TcpListener;
class TCPNETWORKING_API TcpClient;
struct TCPNETWORKING_API NetworkNode;