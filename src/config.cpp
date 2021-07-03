#include "config.h"

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

bool Config::get_bool (std::string identifier) { return map.at (identifier).b (); }

double Config::get_double (std::string identifier) { return map.at (identifier).d (); }

int Config::get_int (std::string identifier) { return (int) map.at (identifier).d (); }

std::string Config::get_string (std::string identifier) { return map.at (identifier).s (); }

void Config::parse (std::string conf_file) {
    try {
        parse_config (conf_file.c_str (), this);
    } catch (std::exception& e) { throw &e; }
}

File_Not_Found::File_Not_Found () : std::runtime_error ("Config file not found") {}
