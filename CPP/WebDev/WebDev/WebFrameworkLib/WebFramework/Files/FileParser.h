#pragma once

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "../Requests/Request.h"

#include <string>

namespace FileParser {
	std::string parseFile(std::string path, Request *request);
	void addContext(std::string &content, Request *request);
	void insertTemplates(std::string &content, Request *request);
}

#endif