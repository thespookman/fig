%language "C++"
%defines
%define parse.trace
%define api.prefix {conf}
%locations

%{
#include "config.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>

extern FILE *confin;

Config* config;

%}

%union {
	char* c;
	double d;
	bool b;
};

%token <c> STRING IDENTIFIER
%token <b> BOOLEAN
%token <d> NUMBER
%token '=' ENDL

%destructor { free($$); } <c>

%{
extern int conflex(conf::parser::semantic_type *conflval, conf::parser::location_type* conflloc);
%}

%start lines
%%

lines	: line			{}
      	| line ENDL		{}
   	| line ENDL lines	{}
	;

line	: IDENTIFIER '=' BOOLEAN	{ config->set(std::string($1), $3); }
     	| IDENTIFIER '=' NUMBER		{ config->set(std::string($1), $3); }
     	| IDENTIFIER '=' STRING		{ config->set(std::string($1), $3); }
     	| error			{}
	;

%%

void parse_config(const char* file_name, Config* _config) {
	config=_config;
	FILE *file=fopen(file_name,"r");
	if(!file) throw File_Not_Found();
	confin=file;
	conf::parser parser;
	parser.parse();
}

namespace conf {
	void parser::error(const location_type& loc, const std::string& msg){}
}
