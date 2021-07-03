#include "config.h"

#include <cstring>

Value::Value (double _v) {
    v.d  = _v;
    type = DOUBLE;
}

Value::Value (int _v) {
    v.d  = _v;
    type = DOUBLE;
}

Value::Value (bool _v) {
    v.b  = _v;
    type = BOOL;
}

Value::Value (const char* _v) {
    v.s  = strdup (_v);
    type = STRING;
}

Value::Value (std::string _v) {
    const char* c = _v.c_str ();
    v.s           = strdup (c);
    type          = STRING;
}

Value::Value (const Value& _v) {
    type = _v.type;
    switch (type) {
    case DOUBLE: v.d = _v.v.d; break;
    case BOOL: v.b = _v.v.b; break;
    case STRING: v.s = strdup (_v.v.s); break;
    }
}

double Value::d () const {
    if (type == Value::DOUBLE) return v.d;
    if (type == Value::BOOL && v.b) return 1;
    return 0;
}

bool Value::b () const {
    if (type == Value::BOOL) return v.b;
    if (type == Value::DOUBLE && v.d != 0) return true;
    return false;
}

std::string Value::s () const {
    if (type == Value::STRING) return std::string (v.s);
    if (type == Value::DOUBLE) return std::to_string (v.d);
    if (v.b) return std::string ("TRUE");
    return std::string ("FALSE");
}

Value::~Value () {
    if (type == STRING) free (v.s);
}
