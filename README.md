# fig
Simple C++ config handler. 

Fig is intended to be foolproof, any bad lines in config files should be ignored, rather than causing issues.

# Usage
Once the Config class has been instantiated, the `set` function can be used to set default values for keys. The `parse` function can then be used to parse a config file. Once that's done, the configured options can be accessed with the `get_bool`, `get_double`, `get_int` and `get_string` functions.  

# Config file format
Fig will treat anything up to the first equals (`=`) on every line as a key (bar any leading or trailing whitespace, and everything after the equals as the value (again bar leading and trailing whitespace).  

Hence `this is a key = this is a value` will be interpreted as a key `"this is a key"` with a string value `"this is a value"`.

`some equals signs======` will be interpreted as a key `"some equals signs"` again with a string value `"====="`.

Values of `true` or `false` (case-insensitive) will be interpreted as boolean.

Numeric values are interpreted as such.
