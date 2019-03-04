#include "WebFramework/Util/Util.h"

#include <istream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

#include <iostream>

void WebServerUtil::replace_all(std::string &str, const std::string &from, const std::string &to) {
	int pos = 0;
	int flen = from.length();
	while ((pos = str.find(from, pos)) != -1) {
		str.replace(pos, flen, to);
		pos += flen;
	}
}

std::map<std::string, std::string> WebServerUtil::parse_attribute_string(std::string str) {
	std::map<std::string, std::string> data;

	std::string key = "", val = "";
	bool addingToKey = true;
	for (std::string::size_type i = 0; i < str.size(); ++i) {
		char c = str[i];
		if (c == '=') {
			addingToKey = false;
			continue;
		}
		else if (c == '&' || i == str.size() - 1) {
			addingToKey = true;
			data[key] = val + c;
			key = "";
			val = "";
			continue;
		}

		if (addingToKey)
			key += c;
		else
			val += c;
	}

	return data;
}

std::string WebServerUtil::FileParser::parseFile(std::string path, Request *request) {
	int code = 404;
	std::string content = "";

	// Read file
	// Open the document in the local file system
	std::ifstream f(path);

	// Check if it opened and if it did, grab the entire contents
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

		insertTemplates(str, request);
		addContext(str, request);
		addStylesheets(str, request);

		content = str;
		code = 200;

		if (request->route == request->server_atts.contextRoute + "/" + request->server_atts.errorFile)
			code = 404;
	}

	f.close();

	request->output_code = code;
	return content;
}

void WebServerUtil::FileParser::addContext(std::string &content, Request *request) {
	for (std::pair<std::string, std::string> pair : request->sendingClient.context) {
		WebServerUtil::replace_all(content, "{{ " + pair.first + " }}", pair.second);
	}
}

void WebServerUtil::FileParser::insertTemplates(std::string &content, Request *request) {
	// get template filepath
	int _pos1 = 0, _pos2 = 0;
	while ((_pos1 = content.find("{{ template(", _pos1)) != -1) {
		_pos1 += 12;
		_pos2 = content.find(") }}", _pos1);
		if (_pos2 == -1)
			break;

		std::string template_filepath = request->server_atts.contextRoute + '/' + request->server_atts.templateRoute + '/' + content.substr(_pos1, _pos2 - _pos1);
		std::string template_content = parseFile(template_filepath, request);

		// get content blocks
		int pos1 = 0, pos2 = 0, pos3 = 0, pos4;
		while ((pos1 = content.find("{{ start_block(", pos1)) != -1) {
			pos1++;
			pos2 = pos1 + 14;
			pos3 = content.find(") }}", pos2);
			if (pos3 == -1)
				break;

			// name
			std::string block_name = content.substr(pos2, pos3 - pos2);

			pos4 = content.find("{{ end_block(" + block_name + ") }}", pos3);
			if (pos4 == -1)
				continue;
			pos3 += 4;

			// content
			std::string block_content = content.substr(pos3, pos4 - pos3);

			// replace
			WebServerUtil::replace_all(template_content, "{{ block(" + block_name + ") }}", block_content);
		}

		content = template_content;
	}
}

void WebServerUtil::FileParser::addStylesheets(std::string &content, Request *request) {
	int _pos1 = 0, _pos2 = 0, _pos3 = 0;
	while ((_pos1 = content.find("{{ style(", _pos1)) != -1) {
		_pos2 = _pos1 + 9;
		_pos3 = content.find(") }}", _pos2);

		std::string style_filepath = request->server_atts.contextRoute + '/' + request->server_atts.stylesheetRoute + '/' + content.substr(_pos2, _pos3 - _pos2);
		
		std::ifstream f(style_filepath);

		// Check if it opened and if it did, grab the entire contents
		if (f.good())
		{
			std::string style_content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			_pos3 += 4;
			WebServerUtil::replace_all(content, content.substr(_pos1, _pos3 - _pos1), "<style>\n" + style_content + "\n</style>");
		}
	}
}