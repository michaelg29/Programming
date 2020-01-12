#include "nums.h"
#include <string>

bool containsNumbers(const char* str) {
	for (char c : std::string(str)) {
		if (c != '.' && (int(c) < int('0') || int(c) > ('9'))) {
			return false;
		}
	}

	return true;
}