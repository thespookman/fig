#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

union V {
	double d;
	bool b;
	char* s;
};

class Value {
       private:
	int type;
	static constexpr int DOUBLE = 0;
	static constexpr int BOOL = 1;
	static constexpr int STRING = 2;
	V v;

       public:
	explicit Value(double _v);
	explicit Value(int _v);
	explicit Value(bool _v);
	explicit Value(std::string _v);
	explicit Value(const char* _v);
	Value(const Value& _v);
	double d() const;
	bool b() const;
	std::string s() const;
	~Value();
};

typedef std::unordered_map<std::string, Value> Confmap;

class Config {
       private:
	Confmap map;

       public:
	Config();
	void set(std::string identifier, Value value);
	void set(std::string identifier, bool value);
	void set(std::string identifier, double value);
	void set(std::string identifier, int value);
	void set(std::string identifier, std::string value);
	bool has(std::string identifier);
	bool get_bool(std::string identifier);
	double get_double(std::string identifier);
	int get_int(std::string identifier);
	std::string get_string(std::string identifier);
	void parse(std::string conf_file);
};

void parse_config(const char* file_name, Config* config);

class File_Not_Found : public std::runtime_error {
       public:
	File_Not_Found();
};

class Value_Not_Found : public std::runtime_error {
       public:
	explicit Value_Not_Found(std::string key);
};
#endif
