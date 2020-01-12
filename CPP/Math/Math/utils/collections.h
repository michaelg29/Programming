#include <vector>
#include <map>
#include <string>

#ifndef COLLECTIONS
#define COLLECTIONS

template<typename T>
bool contains(std::vector<T> v, T x) {
	return std::find(v.begin(), v.end(), x) != v.end();
}

template<typename T>
std::vector<std::string> keys(std::map<std::string, T> v) {
	std::vector<std::string> ret;

	for (auto const& element : v) {
		ret.push_back(element.first);
	}

	return ret;
}

#endif