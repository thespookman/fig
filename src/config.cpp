#include "fig.h"
#include "parser.tab.hh"

Config::Config () {}

void Config::set (std::string identifier, Value value) {
    Confmap::iterator it = map.find (identifier);
    if (it == map.end ())
        map.insert ({identifier, value});
    else
        it->second = value;
}

void Config::set (std::string identifier, bool value) { set (identifier, Value (value)); }

void Config::set (std::string identifier, double value) { set (identifier, Value (value)); }

void Config::set (std::string identifier, int value) { set (identifier, Value (value)); }

void Config::set (std::string identifier, std::string value) { set (identifier, Value (value)); }

bool Config::has (std::string identifier) {
    try {
        map.at (identifier);
    } catch (std::exception& e) { return false; }
    return true;
}

bool Config::get_bool (std::string identifier) {
    try {
        return map.at (identifier).b ();
    } catch (std::out_of_range& e) { throw Value_Not_Found (identifier); }
}

double Config::get_double (std::string identifier) {
    try {
        return map.at (identifier).d ();
    } catch (std::out_of_range& e) { throw Value_Not_Found (identifier); }
}

int Config::get_int (std::string identifier) {
    try {
        return (int) map.at (identifier).d ();
    } catch (std::out_of_range& e) { throw Value_Not_Found (identifier); }
}

std::string Config::get_string (std::string identifier) {
    try {
        return map.at (identifier).s ();
    } catch (std::out_of_range& e) { throw Value_Not_Found (identifier); }
}

void Config::parse (std::string conf_file) {
    try {
        parse_config (conf_file.c_str (), this);
    } catch (std::exception& e) { throw e; }
}

File_Not_Found::File_Not_Found () : std::runtime_error ("Config file not found") {}

Value_Not_Found::Value_Not_Found (std::string key) : std::runtime_error (key + " not found.") {}
