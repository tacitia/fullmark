%option noyywrap

%{
#define MAX_PATH_LENGTH 1024
typedef struct {
  char path[MAX_PATH_LENGTH];
} CharArray;
#define YYSTYPE CharArray
#include "stdlib.h"
#include "stdio.h"
#include "PostfixToPrefix.tab.h"
%}

name	($|_)[a-zA-Z0-9]+
op      [()+\-*/]
space   [ \t]+

%%
{name} 	sprintf(yylval.path, "%s", yytext); return CHAR;
{op}    |
\n      return *yytext;
{space}	/* eats up all white spaces */
.       return ERROR;
%%
