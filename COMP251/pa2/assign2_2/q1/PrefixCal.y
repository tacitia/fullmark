%{
#define YYSTYPE double
#include "stdio.h"
%}

%token NUM
%token ERROR

%%
input: /* empty */ 
     | input line
     ;
line: '\n' 
    | exp '\n' { printf("%.10g\n", $1); }
    ;
exp: NUM { $$ = $1; }
   | '+' exp exp { $$ = $2 + $3; }
   | '-' exp exp { $$ = $2 - $3; }
   | '*' exp exp { $$ = $2 * $3; }
   | '/' exp exp { if ($3 != 0) { $$ = $2 / $3; }
                   else { printf("exception: divide by zero\n"); return 0;}}
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
