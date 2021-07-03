%option noyywrap nounput case-insensitive
%option prefix="conf"
%{
#include <cstdlib>

#include "config.h"
#include "parser.tab.hh"

#define YY_DECL int conflex(conf::parser::semantic_type *conflval, conf::parser::location_type *conflloc)

int yycolumn=1;

#define YY_USER_ACTION conflloc->initialize(NULL, conflineno, yycolumn); conflloc->columns(confleng); yycolumn += confleng;

typedef conf::parser::token token;
%}

%x VALUE
%%

%{
conflloc->step();
%}

"//".*				|
<VALUE,INITIAL>[ \t]		{ conflloc->step(); }
\n				{ ++conflineno; }
"=" 				{ BEGIN(VALUE); return conftext[0]; } 

<VALUE>"true"          		{ conflval->b = true; return token::BOOLEAN; }
<VALUE>"false"         		{ conflval->b = false; return token::BOOLEAN; }
<VALUE>[0-9]*"."?[0-9]+	        { conflval->d = atof(conftext); return token::NUMBER; }
<VALUE>[^\n \t]+[^\n]*[^\n \t]+	{ conflval->c=strdup(conftext); return token::STRING; }

<VALUE>\n			{ ++conflineno; yycolumn = 1; BEGIN(0); return token::ENDL;}

[^\n=]*[^\n= \t]+ 		{ conflval->c=strdup(conftext); return token::IDENTIFIER; }

<<EOF>>				{ return 0; }

%%

