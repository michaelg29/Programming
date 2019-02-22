#include "WebFramework/Sockets/WebServerAttributes.h"

View WebServerAttributes::getView(std::string route) {
	for (std::pair<std::string, View> pair : routes) {
		if (route == "/" + pair.first) {
			return pair.second;
		}
	}

	return nullptr;
}