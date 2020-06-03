#ifndef JSON_HPP
#define JSON_HPP

#include <map>
#include <vector>
#include <string>
#include <initializer_list>
#include <fstream>
#include <sstream>
#include <type_traits>

#define JSON_TRUE "true"
#define JSON_FALSE "false"
#define JSON_NULL "null"

namespace jsoncpp {
	/*
		class callbacks
	*/
	class json;

	/*
		reference to store types of data
	*/
	enum class json_type {
		json_string,
		json_num,
		json_bool,
		json_list,
		json_object,
		json_null
	};

	json_type null = json_type::json_null;

	/*
		utility function callbacks
	*/
	bool firstAndLastMatch(std::string str, char first, char last);
	bool strContains(std::string str, char c);
	bool isNum(char c);
	bool isEmpty(char c);
	bool writeFile(const char* filename, std::string content);
	std::string readFile(const char* filename);
	json parse(std::string val);
	json read(const char* filename);
	bool write(const char* filename, json val, int tabSpace = 0);

	/*
		main class
	*/
	class json {
		/*
			constructors, operator overloading, and data retrieval methods
		*/
	public:
		json_type type; // stores type of data

		// constructors
		json(json_type type = json_type::json_object)								// default
			: type(type) {}
		json(std::string val)														// string
			: type(json_type::json_string), s_val(val) {}
		json(const char* val)														// string
			: type(json_type::json_string), s_val(val) {}
		json(double val)															// number
			: type(json_type::json_num), n_val(val) {}
		json(int val)																// number
			: type(json_type::json_num), n_val(val) {}
		json(float val)																// number
			: type(json_type::json_num), n_val(val) {}
		json(bool val)																// boolean
			: type(json_type::json_bool), b_val(val) {}
		json(std::vector<json> val)													// list
			: type(json_type::json_list), l_val(val) {}
		json(std::initializer_list<json> val)										// list or object
			: type(json_type::json_list), l_val(val) {
			operator=(val);
		}
		json(std::map<std::string, json> val)										// object
			: type(json_type::json_object), o_val(val) {}

		/*
			operator overloading
		*/

		// set methods
		void operator=(std::string val) { type = json_type::json_string; s_val = val; }						// string
		void operator=(const char* val) { type = json_type::json_string; s_val = val; }						// string
		void operator=(double val) { type = json_type::json_num; n_val = val; }								// number
		void operator=(int val) { type = json_type::json_num; n_val = val; }								// number
		void operator=(float val) { type = json_type::json_num; n_val = val; }								// number
		void operator=(bool val) { type = json_type::json_bool; b_val = val; }								// boolean
		void operator=(std::vector<json> val) { type = json_type::json_list; l_val = val; }					// list
		void operator=(std::map<std::string, json> val) { type = json_type::json_object; o_val = val; }		// map
		void operator=(json_type newType) { type = newType; }												// any type (method to set as null)

		/*
			set method using initializer list

			could be two things
			1) json_object
				occurs if each object in the list satisfies all of these conditions:
					a) a json_list (set by default in constructor that takes in initializer list)
					b) has a size of 2
					c) the first value in the list is a string
			2) json_list
				if any of those conditions stated above is not satisfied for any of the objects
		*/
		void operator=(std::initializer_list<json> val) {
			// assume is object
			bool isList = false;

			// placeholder
			std::map<std::string, json> m_val;

			// check to see if all values in list are key-value pairs
			for (json j : val) {
				if (j.type == json_type::json_list &&
					j.l_val.size() == 2 &&
					j.l_val[0].type == json_type::json_string) {
					m_val[j.l_val[0].val<std::string>()] = j.l_val[1];
				}
				else {
					isList = true;
					break;
				}
			}

			if (isList) {
				type = json_type::json_list;
				l_val = val;
			}
			else {
				type = json_type::json_object;
				o_val = m_val;
			}
		}

		// add values
		json& operator+=(const json& val) {
			switch (type) {
			case json_type::json_string:
				// string addition
				switch (val.type) {
				case json_type::json_string:
					s_val += val.s_val;
					break;
				default:
					// TODO: throw error
					break;
				};
				break;
			case json_type::json_num:
				// number addition
				switch (val.type) {
				case json_type::json_num:
					n_val += val.n_val;
					break;
				default:
					// TODO: throw error
					break;
				};
				break;
			case json_type::json_list:
				// list addition
				l_val.push_back(val);
				break;
			};

			return *this;
		}
		friend json operator+(json& val1, json& val2) {
			val1 += val2;
			return val1;
		}

		// add elements of a list to this list
		void addAll(json list) {
			if (type == json_type::json_list && list.type == json_type::json_list) {
				for (json val : list.l_val) {
					l_val.push_back(val);
				}
			}
			else {
				// TODO: throw error
			}
		}

		// data retrieval methods
		/*
			allows for value setting in lists/objects
			eg:
			list = { "first element", 2, false };
			list[1] = true;
			// list	becomes { "first element", true, false }

			obj = {
				"age": 39
			};
			obj["name"] = "Michael";
			obj["age"] = 40;
			// age becomes { "age": 40, "name": "Michael" }
		*/
		json& operator[](int idx) { return l_val[idx]; }							// list
		json& operator[](std::string key) { return o_val[key]; }					// object

		/*
			utility methods
		*/

		// return value of json data
		template <typename T> T val() { return NULL; }
		template <> std::string val<std::string>() { return s_val; }
		template <> const char* val<const char*>() { return s_val.c_str(); }
		template <> double val<double>() { return n_val; }
		template <> int val<int>() { return n_val; }
		template <> float val<float>() { return n_val; }
		template <> bool val<bool>() { return b_val; }

		// returns type as string
		std::string getType() {
			switch (type) {
			case json_type::json_string:
				return "string";
			case json_type::json_num:
				return "number";
			case json_type::json_bool:
				return "bool";
			case json_type::json_list:
				return "list";
			case json_type::json_object:
				return "json object";
			case json_type::json_null:
				return "null";
			}
		}

		/*
			data values
		*/
	private:
		std::string s_val;							// string
		double n_val;								// number
		bool b_val;									// boolean
		std::vector<json> l_val;				// list
		std::map<std::string, json> o_val;		// object

	/*
		parsing methods
	*/
	public:
		// gets json value from string
		/*
			returns vector with data found in string

			parameters
			std::string str - the string containing the object or list
			int& i - reference to the starting index
			bool breakAfterAdd - tells compiler to break the loop after adding one object
				necessary for finding value for dictionary (json_object) because will only pass in one value at a time
			int last_i_offset - how far to stop from end of string
			bool endBracket - if the object will end with a bracket

			default values apply for json_list
			parameters for getting a value in a json_object are as follows:
				breakAfterAdd = true
				last_i_offset = 0
				endBracket = true
		*/
		static std::vector<json> getObj(std::string str, int& i, bool breakAfterAdd = false, int last_i_offset = 1, bool endBracket = false) {
			std::vector<json> ret;

			char c;
			std::string val = "";
			int openCount = 0;
			bool quoteOpen = false;

			std::string openDelimeters = "[{";
			std::string closeDelimeters = "]}";

			for (; i < str.length() - last_i_offset; i++) {
				c = str[i];

				if (isEmpty(c)) {
					// ignore whitespace unless in string
					if (quoteOpen) {
						val += c;
					}
					continue;
				}

				// verify character (cannot be escape char)
				if (str[i - 1] != '\\') {
					if (c == '"') {
						// check to see if valid quotation mark is open or close
						openCount += (quoteOpen ? -1 : 1);

						// reverse boolean value
						quoteOpen = !quoteOpen;
					}

					if (strContains(openDelimeters, c)) {
						// open section
						openCount++;
					}
					else if (strContains(closeDelimeters, c)) {
						// close section
						openCount--;
					}
				}

				// check if end
				if (openCount <= 0 && val != "" && (c == ',' || (endBracket && c == '}'))) {
					// if the current bracket should be included in the object
					if (endBracket && openCount == 0 && c == '}') {
						val += '}';
						i++;
					}

					// parse and then push back the value
					ret.push_back(parse(val));

					// break if only need 1 element (for json_object)
					if (breakAfterAdd) {
						return ret;
					}

					// reset
					val = "";
				}
				else {
					// add character
					val += c;
				}
			}

			// add last value (will only happen in list) because don't break when reach ']'
			ret.push_back(parse(val));

			// return
			return ret;
		}

		/*
			output methods
		*/
	private:
		// stringify json object
		std::string stringify(bool prettify, int tabSpace = 0, int initTabPos = 0, bool isDictVal = false) {
			// return placeholder
			std::stringstream ret;

			// initialize tab string
			std::string tab;
			for (int i = 0; i < tabSpace; i++) { tab += ' '; }

			// indent line if prettify
			if (!isDictVal && prettify) {
				// don't want initial tab indent if dictionary value
				for (int i = 0; i < initTabPos; i++) { ret << tab; }
			}

			std::stringstream sec_stream;
			std::string sec_str;

			// input element
			switch (type) {
			case json_type::json_string:
				// output string val
				ret << '"' << s_val << '"';
				break;
			case json_type::json_num:
				// output numerical val
				ret << n_val;
				break;
			case json_type::json_bool:
				// output boolean value (as true or false) with macros
				ret << (b_val ? JSON_TRUE : JSON_FALSE);
				break;
			case json_type::json_list:
				// if prettify, add new line
				if (prettify) {
					ret << '[' << std::endl;
				}
				else {
					ret << '[';
				}

				for (json data : l_val) {
					// if prettify add prettified element and new line
					if (prettify) {
						sec_stream << data.stringify(true, tabSpace, initTabPos + 1) << ',' << std::endl;
					}
					else {
						sec_stream << data.stringify(false) << ',';
					}

				}
				sec_str = sec_stream.str();
				sec_str = sec_str.substr(0, sec_str.length() - (prettify ? 2 : 1)); // if prettify, will have a new line at the end with the comma

				// if prettify add new line and tab so the closing bracket is inline
				if (prettify) {
					ret << sec_str << std::endl;
					for (int i = 0; i < initTabPos; i++) { ret << tab; }
				}
				else {
					ret << sec_str;
				}

				// closing bracket
				ret << ']';
				break;
			case json_type::json_object:
				// if prettify add new line
				if (prettify) {
					ret << '{' << std::endl;
				}
				else {
					ret << '{';
				}

				for (std::pair<std::string, json> pair : o_val) {
					// if prettify add prettified element and new line
					if (prettify) {
						for (int i = 0; i < initTabPos + 1; i++) { sec_stream << tab; }
						sec_stream << '"' << pair.first << "\": " << pair.second.stringify(true, tabSpace, initTabPos + 1, true) << ',' << std::endl;
					}
					else {
						sec_stream << '"' << pair.first << "\":" << pair.second.stringify(false) << ',';
					}
				}
				sec_str = sec_stream.str();
				sec_str = sec_str.substr(0, sec_str.length() - (prettify ? 2 : 1)); // if prettify, will have a new line at the end with the comma

				// if prettify add new line and tab so the closing bracket is inline
				if (prettify) {
					ret << sec_str << std::endl;
					for (int i = 0; i < initTabPos; i++) { ret << tab; }
				}
				else {
					ret << sec_str;
				}

				// closing bracket
				ret << '}';
				break;
			case json_type::json_null:
				// output null with macro
				ret << JSON_NULL;
				break;
			}

			return ret.str();
		}
	public:
		// public stringify method
		std::string dump(int tabSpace = 0) {
			if (tabSpace == 0) {
				// don't prettify
				return stringify(false);
			}
			else {
				return stringify(true, tabSpace);
			}
		}

		// write to file
		bool dump(const char* filename, int tabSpace = 0) {
			return writeFile(filename, dump(tabSpace));
		}
	};

	/*
		utility method definitions (callbacks above)
	*/

	// determines if first and last character of a string match the parameters
	bool firstAndLastMatch(std::string str, char first, char last) {
		return str[0] == first && str[str.length() - 1] == last;
	}

	// determines if string contains a character
	bool strContains(std::string str, char c) {
		return str.find(c) != std::string::npos;
	}

	// determines if character is a digit, decimal point, or negative sign
	bool isNum(char c) {
		return strContains("0123456789.-", c);
	}

	// determines if character is whitespace
	bool isEmpty(char c) {
		return c == ' ' || c == '\n' || c == '\t';
	}

	// writes string to specified file
	bool writeFile(const char* filename, std::string content) {
		std::ofstream file(filename);

		if (file.is_open()) {
			file << content;
			file.close();
			return true;
		}

		return false;
	}

	// returns contents as string from specified file
	std::string readFile(const char* filename) {
		std::ifstream file(filename);
		std::string ret = "";

		if (file.is_open()) {
			std::stringstream sstream;
			sstream << file.rdbuf();
			ret = sstream.str();
		}

		file.close();

		return ret;
	}

	// parse json object
	json parse(std::string val) {
		// identify type based on first character
		char c = val[0];

		if (firstAndLastMatch(val, '"', '"')) {
			// string
			return json(val.substr(1, val.length() - 2));
		}
		else if (isNum(c)) {
			// number
			return json(std::atof(val.c_str()));
		}
		else if (c == 't') {
			// might be true
			if (val == JSON_TRUE) {
				return json(true);
			}
		}
		else if (c == 'f') {
			// might be false
			if (val == JSON_FALSE) {
				return json(false);
			}
		}
		else if (c == 'n') {
			// might be null
			if (val == JSON_NULL) {
				return json(json_type::json_null);
			}
		}
		else if (firstAndLastMatch(val, '[', ']')) {
			// list

			// index value
			int i = 1;

			return json::getObj(val, i);
		}
		else if (firstAndLastMatch(val, '{', '}')) {
			// json object

			// return placeholder
			std::map<std::string, json> ret;

			// index value
			int i = 1;

			// key placeholder
			std::string key = "";

			while (i < val.length()) {
				/*
					get key
				*/

				c = val[i];

				// get first character
				while (isEmpty(c) && i < val.length()) {
					c = val[++i];
				}

				if (c != '"') {
					// invalid key
					break;
				}

				c = val[++i];

				// find end of string while c not valid quote
				for (; c != '"' && val[i - 1] != '\\'; c = val[++i]) {
					key += c;
				}

				c = val[++i];

				// skip to item
				while (isEmpty(c) || c == ':') {
					c = val[++i];
				}

				/*
					get value
				*/

				std::vector<json> retv = json::getObj(val, i, true, 0, true);

				i++;

				// set key and value
				ret[key] = retv[0];

				// reset placeholders
				key = "";
			}

			return ret;
		}

		// default return
		return json();
	}

	// read json object from file
	json read(const char* filename) {
		return parse(readFile(filename));
	}

	// write json object to file
	bool write(const char* filename, json val, int tabSpace) {
		return writeFile(filename, val.dump(tabSpace));
	}
}

#endif // JSON_HPP