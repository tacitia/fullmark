%option noyywrap

%{
#define YYSTYPE double
#include "stdlib.h"
#include "PrefixCal.tab.h"
%}

real    -?(0*\.[0-9]*[1-9]+[0-9]*|[0-9]*[1-9]+[0-9]*\.?[0-9]*)
op	[+\-*/]
space	[ \t]+

%%
{real} 	yylval = atof(yytext); return NUM;
{op}	|
\n	return *yytext;
{space} /* eats up all white spaces */
.       return ERROR;
%%

