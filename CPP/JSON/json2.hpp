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

namespace jsoncpp2 {
    /*
        class callbacks
    */
    class json;

    /*
        reference to store types of data
    */
    enum class json_type {
        json_string,
        json_float,
        json_int,
        json_bool,
        json_list,
        json_object,
        json_null
    };

    const json_type null = json_type::json_null;

    /*
        utility function callbacks
    */
    inline bool firstAndLastMatch(std::string str, char first, char last);
    inline bool strContains(std::string str, char c);
    inline bool isNum(char c);
    inline bool isEmpty(char c);
    inline bool writeFile(const char* filename, std::string content);
    inline std::string readFile(const char* filename);
    inline json parse(std::string val);
    inline json read(const char* filename);
    inline bool write(const char* filename, json val, int tabSpace = 0);

    /*
        main class
    */
    class json {
        union json_val {
            std::string s;
            float r;
            int i;
            bool b;
            std::vector<json> l;
            std::map<std::string, json> o;
            void* n;

            json_val() {}
            ~json_val() {}

            json_val(const std::string& str) : s(str) {}
            json_val& operator=(const std::string& str) { new(&s) std::string(str); return *this; }

            json_val(float real) : r(real) {}
            json_val& operator=(float real) { r = real; return *this; }

            json_val(double real) : r(real) {}
            json_val& operator=(double real) { r = real; return *this; }

            json_val(int integer) : i(integer) {}
            json_val& operator=(int integer) { i = integer; return *this; }

            json_val(bool boolean) : b(boolean) {}
            json_val& operator=(bool boolean) { b = boolean; return *this; }

            json_val(const std::vector<json>& list) : l(list) {}
            json_val& operator=(const std::vector<json>& list) { new(&l) std::vector<json>(list); return *this; }

            json_val(const std::map<std::string, json>& obj) : o(obj) {}
            json_val& operator=(const std::map<std::string, json>& obj) {
                if (obj.size() == 0) {
                    new(&o) std::map<std::string, json>();
                }
                else {
                    new(&o) std::map<std::string, json>(obj);
                }
                return *this;
            }

            void setNull() { this->n = NULL; }
        };

        /*
            constructors, operator overloading, and data retrieval methods
        */
    public:
        json_type type; // stores type of data
        json_val jval;

        json(const json& obj) { operator=(obj); }

        // constructors
        json(json_type type = json_type::json_object)								// default
            : type(type) {}
        json(std::string val)														// string
            : type(json_type::json_string) {
            jval = val;
        }
        json(const char* val)														// string
            : json(std::string(val)) {}
        json(double val)															// float
            : type(json_type::json_float) {
            jval.r = val;
        }
        json(float val)																// float
            : type(json_type::json_float) {
            jval.r = val;
        }
        json(int val)																// integer
            : type(json_type::json_int) {
            jval.i = val;
        }
        json(bool val)																// boolean
            : type(json_type::json_bool) {
            jval.b = val;
        }
        json(std::vector<json> val)													// list
            : type(json_type::json_list) {
            parseList(val);
        }
        json(std::initializer_list<json> val)										// list or object
        {
            parseList(std::vector<json>(val));
        }
        json(std::map<std::string, json> val)										// object
            : type(json_type::json_object) {
            jval = val;
        }

        /*
            operator overloading
        */

        // set methods
        void operator=(const json& obj) {
            type = obj.type;
            switch (type) {
            case json_type::json_string:
                jval = obj.jval.s; break;
            case json_type::json_float:
                jval.r = obj.jval.r; break;
            case json_type::json_int:
                jval.i = obj.jval.i; break;
            case json_type::json_bool:
                jval.b = obj.jval.b; break;
            case json_type::json_list:
                jval = obj.jval.l; break;
            case json_type::json_object:
                jval = obj.jval.o; break;
            case json_type::json_null:
                jval.n = NULL;
            }
        }
        void operator=(std::string val) { type = json_type::json_string; jval.s = val; }						// string
        void operator=(const char* val) { operator=(std::string(val)); }						// string
        void operator=(double val) { type = json_type::json_float; jval.r = val; }							// float
        void operator=(float val) { type = json_type::json_float; jval.r = val; }							// float
        void operator=(int val) { type = json_type::json_int; jval.i = val; }								// int
        void operator=(bool val) { type = json_type::json_bool; jval.b = val; }								// boolean
        void operator=(std::vector<json> val) { type = json_type::json_list; parseList(val); }					// list
        void operator=(std::map<std::string, json> val) { type = json_type::json_object; jval.o = val; }		// map
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
            operator=(std::vector<json>(val));
        }

        void parseList(std::vector<json> val) {
            // assume is object
            bool isList = false;

            // placeholder
            std::map<std::string, json> m_val;

            // check to see if all values in list are key-value pairs
            for (json j : val) {
                if (j.type == json_type::json_list &&
                    j.jval.l.size() == 2 &&
                    j.jval.l[0].type == json_type::json_string) {
                    m_val[j.jval.l[0].val<std::string>()] = j.jval.l[1];
                }
                else {
                    isList = true;
                    break;
                }
            }

            if (isList) {
                type = json_type::json_list;
                jval = val;
            }
            else {
                type = json_type::json_object;
                jval = m_val;
            }
        }

        // equality operator
        friend bool operator==(const json& lhs, const json& rhs) {
            // test type
            if (lhs.type != rhs.type) {
                return false;
            }

            switch (lhs.type) {
            case json_type::json_string:
                return lhs.jval.s == rhs.jval.s;
            case json_type::json_float:
                return lhs.jval.r == rhs.jval.r;
            case json_type::json_int:
                return lhs.jval.i == rhs.jval.i;
            case json_type::json_bool:
                return lhs.jval.b == rhs.jval.b;
            case json_type::json_list:
                return lhs.jval.l == rhs.jval.l;
            case json_type::json_object:
                return lhs.jval.o == rhs.jval.o;
            case json_type::json_null:
                return true; // both are null
            }
        }

        // add values
        json& operator+=(const json& val) {
            switch (type) {
            case json_type::json_string:
                // string addition
                if (val.type == json_type::json_string) {
                    jval.s += val.jval.s;
                }
                else {
                    // TODO: throw error
                }
                break;
            case json_type::json_float:
                // number addition
                if (val.type == json_type::json_float) {
                    jval.r += val.jval.r;
                }
                else if (val.type == json_type::json_int) {
                    jval.i += val.jval.i;
                }
                else {
                    // TODO: throw error
                }
                break;
            case json_type::json_int:
                // number addition
                if (val.type == json_type::json_int) {
                    jval.i += val.jval.i;
                }
                else if (val.type == json_type::json_float) {
                    jval.i += val.jval.r;
                }
                else {
                    // TODO: throw error
                }
                break;
            case json_type::json_list:
                // list addition
                jval.l.push_back(val);
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
                for (json val : list.jval.l) {
                    jval.l.push_back(val);
                }
            }
            else {
                // TODO: throw error
            }
        }

        // remove object at key in dictionary
        bool eraseKey(std::string key) {
            if (type == json_type::json_object) {
                return jval.o.erase(key) != 0;
            }
            else {
                // TODO: throw error
                return false;
            }
        }

        // remove object from list
        bool erase(json item) {
            if (type == json_type::json_list) {
                unsigned int size = jval.l.size();
                for (int i = 0; i < size; i++) {
                    if (jval.l[i] == item) {
                        jval.l.erase(jval.l.begin() + i);
                        return true;
                    }
                }
                return false;
            }
            else {
                // TODO: throw error
                return false;
            }
        }

        // get length of list
        unsigned int length() {
            if (type == json_type::json_list) {
                return jval.l.size();
            }
            else {
                // TODO: throw error
                return -1;
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
        json& operator[](int idx) { return jval.l[idx]; }							// list
        json& operator[](std::string key) { return jval.o[key]; }					// object

        /*
            utility methods
        */

        // return value of json data
        template <typename T> T val() { return NULL; }
        template <> std::string val<std::string>() { return jval.s; }
        template <> const char* val<const char*>() { return jval.s.c_str(); }
        template <> double val<double>() {
            return type == json_type::json_float ? jval.r : jval.i;
        }
        template <> float val<float>() {
            return type == json_type::json_float ? jval.r : jval.i;
        }
        template <> int val<int>() {
            return type == json_type::json_int ? jval.i : jval.r;
        }
        template <> bool val<bool>() { return jval.b; }

        // returns type as string
        std::string getType() {
            switch (type) {
            case json_type::json_string:
                return "string";
            case json_type::json_float:
                return "float";
            case json_type::json_int:
                return "int";
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
            if (val != "") {
                ret.push_back(parse(val));
            }
            else {
                int i;
            }

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
                ret << '"' << jval.s << '"';
                break;
            case json_type::json_float:
                // output float val
                ret << jval.r;
                break;
            case json_type::json_int:
                // output integer val
                ret << jval.i;
                break;
            case json_type::json_bool:
                // output boolean value (as true or false) with macros
                ret << (jval.b ? JSON_TRUE : JSON_FALSE);
                break;
            case json_type::json_list:
                // if prettify, add new line
                if (prettify) {
                    ret << '[' << std::endl;
                }
                else {
                    ret << '[';
                }

                for (json data : jval.l) {
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

                for (std::pair<std::string, json> pair : jval.o) {
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
    inline bool firstAndLastMatch(std::string str, char first, char last) {
        return str[0] == first && str[str.length() - 1] == last;
    }

    // determines if string contains a character
    inline bool strContains(std::string str, char c) {
        return str.find(c) != std::string::npos;
    }

    // determines if character is a digit, decimal point, or negative sign
    inline bool isNum(char c) {
        return strContains("0123456789.-", c);
    }

    // determines if character is whitespace
    inline bool isEmpty(char c) {
        return c == ' ' || c == '\n' || c == '\t';
    }

    // writes string to specified file
    inline bool writeFile(const char* filename, std::string content) {
        std::ofstream file(filename);

        if (file.is_open()) {
            file << content;
            file.close();
            return true;
        }

        return false;
    }

    // returns contents as string from specified file
    inline std::string readFile(const char* filename) {
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
    inline json parse(std::string val) {
        // identify type based on first character
        char c = val[0];

        if (firstAndLastMatch(val, '"', '"')) {
            // string
            return json(val.substr(1, val.length() - 2));
        }
        else if (isNum(c)) {
            // number
            if (strContains(val, '.')) {
                // float
                return json(std::atof(val.c_str()));
            }
            else {
                // integer
                return json(std::atoi(val.c_str()));
            }
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
    inline json read(const char* filename) {
        return parse(readFile(filename));
    }

    // write json object to file
    inline bool write(const char* filename, json val, int tabSpace) {
        return writeFile(filename, val.dump(tabSpace));
    }
}

#endif // JSON_HPP