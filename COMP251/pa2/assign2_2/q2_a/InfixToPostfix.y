%{
#define MAX_PATH_LENGTH 1024
typedef struct {
  char path[MAX_PATH_LENGTH];
} CharArray;
#define YYSTYPE CharArray
#include "stdio.h"
#include "stdlib.h"
%}

%token ERROR
%token CHAR
%left '-' '+'
%left '*' '/'

%%
input: /* empty */ 
     | input line
     ;
line: '\n'
    | exp '\n' { printf("%s\n", $1.path); }
    ;
exp: CHAR { sprintf($$.path, "%s", $1.path); }
   | exp '+' exp { sprintf($$.path, "%s %s +", $1.path, $3.path); }
   | exp '-' exp { sprintf($$.path, "%s %s -", $1.path, $3.path); }
   | exp '*' exp { sprintf($$.path, "%s %s *", $1.path, $3.path); }
   | exp '/' exp { sprintf($$.path, "%s %s /", $1.path, $3.path); }
   | '(' exp ')' { sprintf($$.path, "%s", $2.path); }
   ;
%%

int main()
{
  return yyparse();
}

int yyerror(const char* s)
{
  printf("%s\n", s);
  return 0;
}	

