#pragma once

#define WEBFRAMEWORK_API __declspec(dllexport)

// data
class WEBFRAMEWORK_API Global;

// server and client bodies
class WEBFRAMEWORK_API TcpListener;
class WEBFRAMEWORK_API WebServer;
struct WEBFRAMEWORK_API WebClient;
struct WEBFRAMEWORK_API WebServerAttributes;

// requests
class WEBFRAMEWORK_API Request;