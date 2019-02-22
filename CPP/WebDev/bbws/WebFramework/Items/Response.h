#pragma once

#ifndef RESPONSE_H
#define RESPONSE_H

struct Response {
public:
	const char* content;
	int size;

	Response(const char* content, int size) :
		content(content), size(size) { }
};

#endif RESPONSE_H