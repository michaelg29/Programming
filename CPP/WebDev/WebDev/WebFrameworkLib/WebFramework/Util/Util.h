#pragma once

#ifndef GLOBAL_H
#define GLOBAL_H

#include "../WebFramework.h"
#include "../Sockets/WebServer.h"

#include <string>
#include <map>

namespace WebServerUtil {
	void replace_all(std::string &str, const std::string &from, const std::string &to);
}

#endif