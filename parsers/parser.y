//	Parser

%{
#include <stdio.h>
#include "lex.yy.c"
%}

%token tokNumber, tokFunction, tokOperator, tokErr, tokEOF, tokVariable

%%
Start:			Expression ';' {
					cout<<"\nParse successful\n";
					return 0;
				}
				;

Expression:		Component tokOperator Expression
			|	Component
			;

BracketedExpression:	'(' Expression ')';

Component:		tokVariable
			|	tokNumber
			|	BracketedExpression
			|	Function
			;

Function:		tokFunction BracketedExpression;
%%

int yyerror(char *err)
{
	char str[50];
	
	if(yytext[0]==';')
		cerr<<err<<": near end of input"<<endl;
	else
		cerr<<err<<": near symbol "<<yytext<<endl;
	e->setError();
	
	return 1;
}
