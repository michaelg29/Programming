#include "WebFramework/Util/Util.h"

void WebServerUtil::replace_all(std::string &str, const std::string &from, const std::string &to) {
	int pos = 0;
	int flen = from.length();
	while ((pos = str.find(from, pos)) != -1) {
		str.replace(pos, flen, to);
		pos += flen;
	}
}