#ifndef CONSOLETOOLS_HPP
#define CONSOLETOOLS_HPP

#include <time.h>
#include <iostream>
#include <string>
#include <sstream>

class App
{
public:
	App(std::string name, std::string author)
		: name(name), author(author) {
		std::cout << "====================" << std::endl
			<< name << std::endl
			<< "By " << author << std::endl
			<< "====================" << std::endl
			<< "Type 'stop' to finish" << std::endl << std::endl;
	}

	void init() {
		start = time(NULL);
	}

	void cleanup() {
		end = time(NULL);

		char str[26];
		ctime_s(str, sizeof(str), &start);
		std::cout << "Session started at: " << str << std::endl;

		ctime_s(str, sizeof(str), &end);
		std::cout << "Session ended at:   " << str << std::endl;

		time_t seconds(difftime(end, start));
		tm duration;
		gmtime_s(&duration, &seconds);
		std::cout << "Total elapsed time: " << printDate(duration) << std::endl;
	}

	static std::string printDate(tm date) {
		std::stringstream ret;

		ret << pad(date.tm_year - 70, 2) << ':' // -70 because date starts at 1970 and tm_year gives number of years since 1900
			<< pad(date.tm_yday, 3) << ':'
			<< pad(date.tm_hour, 2) << ':'
			<< pad(date.tm_min, 2) << ':'
			<< pad(date.tm_sec, 2);

		return ret.str();
	}

	template<typename T>
	static std::string pad(T val, int length, char pad = '0') {
		std::string val_s = std::to_string(val);

		std::string ret;

		for (int i = 0, noChars = length - val_s.length(); i < noChars; i++) {
			ret += pad;
		}

		ret += val_s;

		return ret;
	}

private:
	std::string name;
	std::string author;

	time_t start;
	time_t end;
};

#endif