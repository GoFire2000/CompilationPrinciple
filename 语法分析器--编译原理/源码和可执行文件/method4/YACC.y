%{

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE double	/* doble type for YACC stack; for yylval */

int yylex(void);
int yyerror(char *);

%}

%token NUM

%%

line	:	line expr '\n'	{printf("Success!\n");}
     	|	line '\n'
	|	/* error */
	|	error '\n'	{yyerror("reenter last line:");}
    	;
expr	:	expr '+' term	{$$ = $1 + $3;	printf("E->E+T\n");}
     	|	expr '-' term	{$$ = $1 - $3;	printf("E->E-T\n");}
	|	term		{		printf("E->T\n");}
	;
term	:	term '*' factor	{$$ = $1 * $3;	printf("T->T*F\n");}
     	|	term '/' factor	{$$ = $1 / $3;	printf("T->T/F\n");}
	|	factor		{		printf("T->F\n");}
	;
factor	:	'(' expr ')'	{$$ = $2;	printf("F->(E)\n");}
       	|	NUM	{		printf("F->num\n");}
	;

%%

int yylex() {
  int c;
  while ((c = getchar()) == ' ') ;
  if (c == '.' || isdigit(c)) {
    ungetc(c, stdin);
    scanf("%lf", &yylval);
    return NUM;
  }
  return c;
}

int yyerror(char *s) {
  fprintf(stderr, "error: %s\n", s);
}

int main()
{
  puts("Input your string: ");
  return yyparse();
}
