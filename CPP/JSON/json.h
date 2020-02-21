#ifndef JSON_H
#define JSON_H

#include <map>
#include <vector>
#include <string>

namespace json {
	enum class json_type {
		json_string,
		json_num,
		json_bool,
		json_list,
		json_object
	};

	class json_data {
	public:
		static json_data parse(std::string str);

		json_data(json_type type = json_type::json_object)
			: type(type) {}
		json_data(std::string val)
			: type(json_type::json_string), s_val(val) {}
		json_data(double val)
			: type(json_type::json_num), n_val(val) {}
		json_data(bool val)
			: type(json_type::json_bool), b_val(val) {}
		json_data(std::vector<json_data> val)
			: type(json_type::json_list), l_val(val) {}
		json_data(std::map<std::string, json_data> val)
			: type(json_type::json_object), o_val(val) {}

		std::string stringify();

		json_type type;
		std::string getType();

	protected:
		std::string s_val;
		double n_val;
		bool b_val;
		std::vector<json_data> l_val;
		std::map<std::string, json_data> o_val;
	};

	class json_string : public json_data {
	public:
		json_string(std::string val)
			: json_data(val) {}

		std::string stringify();
		std::string val() { return s_val; }
	};

	class json_num : public json_data {
	public:
		json_num(double val)
			: json_data(val) {}

		std::string stringify();
		double val() { return n_val; }
	};

	class json_bool : public json_data {
	public:
		static std::string TRUE;
		static std::string FALSE;

		json_bool(bool val)
			: json_data(val) {}

		std::string stringify();
		bool val() { return b_val; }
	};

	class json_list : public json_data {
	public:
		json_list(std::vector<json_data> val)
			: json_data(val) {}

		std::string stringify();
		std::vector<json_data> val() { return l_val; }
	};

	class json_object : public json_data {
	public:
		json_object(std::map<std::string, json_data> val)
			: json_data(val) {}

		std::string stringify();
		std::map<std::string, json_data> val() { return o_val; }
	};

	class json_util {
	public:
		static json_data parse(std::string json_str);

		static json_data read(const char* filename);
		static std::string readFile(const char* filename);

		static std::string stringify(json_data data);

		static bool write(const char* filename, json_data val);
		static bool writeFile(const char* filename, std::string content);
	};
}

#endif