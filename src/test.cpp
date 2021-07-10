#include "fig.h"

#include <iostream>

void describe (std::string description, bool pass, const std::string& info) {
    std::cout << description;
    for (unsigned int i = description.size (); i < 80; ++i) { std::cout << " "; }
    if (pass)
        std::cout << "\e[32m✔\e[0m" << std::endl;
    else {
        std::cout << "\e[31m✖\e[0m" << std::endl;
        std::cout << "\t" << info << std::endl;
    }
}

void assert_equals_bool (Config* conf, const std::string& description, std::string key,
                         bool expected, bool* result) {
    bool        pass  = false;
    std::string error = "";
    try {
        bool actual = conf->get_bool (key);
        if (actual == expected)
            pass = true;
        else
            error = std::string ("Expected ") + (expected ? "TRUE" : "FALSE") +
                    " for key value \"" + key + "\". Got " + (actual ? "TRUE" : "FALSE") + ".";
    } catch (std::exception& e) { error = e.what (); }
    describe (description, pass, error);
    if (*result && !pass) *result = false;
}

void assert_equals_double (Config* conf, const std::string& description, std::string key,
                           double expected, bool* result) {
    bool        pass  = false;
    std::string error = "";
    try {
        double actual = conf->get_double (key);
        if (actual == expected)
            pass = true;
        else
            error = std::string ("Expected ") + std::to_string (expected) + " for key value \"" +
                    key + "\". Got " + std::to_string (actual) + ".";
    } catch (std::exception& e) { error = e.what (); }

    describe (description, pass, error);
    if (*result && !pass) *result = false;
}

void assert_equals_int (Config* conf, const std::string& description, std::string key, int expected,
                        bool* result) {
    bool        pass  = false;
    std::string error = "";
    try {
        int actual = conf->get_int (key);
        if (actual == expected)
            pass = true;
        else
            error = std::string ("Expected ") + std::to_string (expected) + " for key value \"" +
                    key + "\". Got " + std::to_string (actual) + ".";
    } catch (std::exception& e) { error = e.what (); }

    describe (description, pass, error);
    if (*result && !pass) *result = false;
}

void assert_equals_string (Config* conf, const std::string& description, std::string key,
                           const std::string& expected, bool* result) {
    bool        pass  = false;
    std::string error = "";
    try {
        std::string actual = conf->get_string (key);
        if (actual.compare (expected) == 0)
            pass = true;
        else
            error = std::string ("Expected \"") + expected + "\" for key value \"" + key +
                    "\". Got \"" + actual + "\".";
    } catch (std::exception& e) { error = e.what (); }

    describe (description, pass, error);
    if (*result && !pass) *result = false;
}

void assert_exists (Config* conf, const std::string& description, std::string key, bool expected,
                    bool* result) {
    bool        pass  = false;
    std::string error = "";
    try {
        bool actual = conf->has (key);
        if (actual == expected)
            pass = true;
        else
            error = std::string ("Expected \"") + key + "\" to " + (expected ? "" : "not ") +
                    "exist, but it does " + (actual ? "" : "not ") + "exist.";
    } catch (std::exception& e) { error = e.what (); }

    describe (description, pass, error);
    if (*result && !pass) *result = false;
}

void assert_reads_file (Config* conf, const std::string& description, const std::string& file,
                        bool expected, bool* result) {
    bool        pass   = false;
    std::string error  = "";
    bool        actual = false;
    try {
        conf->parse (file);
        actual = true;
        if (actual == expected)
            pass = true;
        else
            error =
                std::string ("Shouldn't be able to read ") + file + ", but can. Something's off...";
    } catch (std::exception& e) {
        actual = false;
        if (actual == expected)
            pass = true;
        else
            error = e.what ();
    }
    describe (description, pass, error);
};

int main (int argc, char** argv) {
    std::cout << std::endl << "Starting test run" << std::endl << std::endl;
    bool   passing = true;
    Config conf;
    assert_reads_file (&conf, "Reads a valid file", "config_1", true, &passing);
    assert_exists (&conf, "Reads a simple key", "onewordkey", true, &passing);
    assert_exists (&conf, "Doesn't read a non-existant key", "doesn't exist", false, &passing);
    assert_exists (&conf, "Reads a key with spaces", "key with spaces", true, &passing);
    assert_exists (&conf, "Reads a key with trailing space", "trailing space in key", true,
                   &passing);
    assert_exists (&conf, "Reads a key with leading space", "leading space in key", true, &passing);
    assert_exists (&conf, "Reads a key with lots of leading and trailing space",
                   "lots of leading and trailing space, including tabs", true, &passing);
    assert_exists (&conf, "Doesn't read a line with no equals",
                   "No equals, this line should be ignored", false, &passing);
    assert_equals_int (&conf, "Reads an integer", "integer", 21, &passing);
    assert_equals_int (&conf, "Reads a negative integer", "negative integer", -42, &passing);
    assert_equals_int (&conf, "Reads an integer with spaces", "integer with spaces", 84, &passing);
    assert_equals_double (&conf, "Reads a double", "double", 4.2, &passing);
    assert_equals_double (&conf, "Reads a negative double", "negative double", -35.1, &passing);
    assert_equals_double (&conf, "Reads a double with spaces", "double with spaces", 405.4,
                          &passing);
    assert_equals_bool (&conf, "Reads a boolean true", "boolean true", true, &passing);
    assert_equals_bool (&conf, "Reads a boolean true in capitals", "BOOLEAN TRUE", true, &passing);
    assert_equals_bool (&conf, "Reads a boolean false", "boolean false", false, &passing);
    assert_equals_bool (&conf, "Reads a boolean false in capitals", "BOOLEAN FALSE", false,
                        &passing);
    assert_equals_bool (&conf, "Reads a boolean true with spaces", "true with spaces", true,
                        &passing);
    assert_equals_bool (&conf, "Reads a boolean false with spaces", "false with spaces", false,
                        &passing);
    assert_equals_bool (&conf, "Reads a boolean in mixed case", "mixed case", true, &passing);
    assert_equals_string (&conf, "Reads a simple string", "string", "some text", &passing);
    assert_equals_string (&conf, "Reads a string with leading and trailing space",
                          "string with leading and trailing",
                          "Some text with leading and trailing whitespace", &passing);
    assert_equals_string (&conf, "Reads a string with equals signs", "string containing equals",
                          "= = = =", &passing);
    assert_exists (&conf, "Doesn't read values without keys", "", false, &passing);
    Config conf2;
    assert_reads_file (&conf2, "Reads an empty file", "config_2", true, &passing);
    assert_exists (&conf2, "Doesn't create empty keys", "", false, &passing);
    Config conf3;
    assert_reads_file (&conf3, "Throws when reading non-existant file", "config_3", false,
                       &passing);
    Config conf4;
    conf4.set ("int", 0);
    conf4.set ("bool", true);
    conf4.set ("string", "str");
    assert_reads_file (&conf4, "Can overwrite preset values", "config_4", true, &passing);
    assert_equals_int (&conf4, "Integer overwritten correctly", "int", 1, &passing);
    assert_equals_bool (&conf4, "Boolean overwritten correctly", "bool", false, &passing);
    assert_equals_string (&conf4, "String overwritten correctly", "string", "string", &passing);
    std::cout << std::endl;
    if (passing)
        std::cout << "\e[32mTests Passed\e[0m" << std::endl;
    else
        std::cout << "\e[31mTests Failed\e[0m" << std::endl;
    return (passing ? 0 : 1);
}
