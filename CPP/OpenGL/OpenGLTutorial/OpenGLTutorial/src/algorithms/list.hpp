#ifndef LIST_HPP
#define LIST_HPP

namespace List {
	// find index of item in vector list
	template<typename T>
	int getIndexOf(std::vector<T> v, T x) {
		return std::find(v.begin(), v.end(), x) - v.begin();
	}

	// erase item in vector list
	template<typename T>
	void remove(std::vector<T>* v, T target) {
		v->erase(std::find(v->begin(), v->end(), target));
	}

	// test if vector list contains item
	template<typename T>
	bool contains(std::vector<T> v, T x) {
		return getIndexOf<T>(v, x) != v.size();
	}
}

#endif