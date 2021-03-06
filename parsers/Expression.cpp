//	Expression class
/*
	This class stores an expression in upto 6 variables (x, y, z, t0, t1, t2).
	Methods include:
		Adding a token
		Evaluating the expression for a given set of values for variables
*/

#ifndef _EXPRESSION
#define _EXPRESSION

//
using namespace std;
#include <iostream>
//

#include <string.h>
#include "precedence.cpp"
#include "function.cpp"
#include "Stack.cpp"
#include "ExpHead.cpp"
#include "parser.tab.h"

class Expression {
	public:
		enum FunctionType {
			FTYPE_1,		//	f(x, y, z, s)
			FTYPE_2,		//	f(x, y, z)
			FTYPE_3,		//	f(t)
			FTYPE_UNKNOWN
		};
	
	private:
		ExpHead *expression;
		FunctionType type;
		Stack<char> *cStack;
		Stack<ExpHead*> *subStack;
		void extend(char token[], int type);
		
	public:
		Expression();
		Expression(FunctionType ft);
		~Expression();
		void addToken(char token[], int type);
		double evaluate(double val[]);
		void display();
		void parse(char *str);
		int error();
		void setError();
		void functionType(FunctionType ft);
		FunctionType functionType();
} *e;

#include "parser.tab.c"

/*	Given a token and type of token, append it to the 
	postfix representation of the expression	*/
void Expression::extend(char token[], int type)
{
	Token *temp=new Token();
	
	if(subStack->count()==0)
		subStack->push(expression);
	
	switch(type)
	{
		case tokNumber:
		{
			temp->type=Token::TOK_NUM;
			temp->value.number=strtod(token, NULL);
			subStack->peek()->extend(temp);
			break;
		}
		
		case tokVariable:
			temp->type=Token::TOK_VAR;
			
			switch(token[0]) {
				case 'x':
					temp->value.variable=Token::VAR_X;
					break;
				case 'y':
					temp->value.variable=Token::VAR_Y;
					break;
				case 'z':
					temp->value.variable=Token::VAR_Z;
					break;
				case 't':
					temp->value.variable=Token::VAR_T;
					break;
				case 's':
					temp->value.variable=Token::VAR_S;
					break;
			}
			
			subStack->peek()->extend(temp);
			break;
			
		case tokOperator:
			temp->type=Token::TOK_OP;
			temp->value.oper=token[0];
			subStack->peek()->extend(temp);
			break;
			
		case tokFunction:
		{
			ExpHead *etemp=new ExpHead(ExpHead::EXP_FUNCTION, token);
			
			temp->type=Token::TOK_SUB;
			temp->value.subExp=etemp;
			subStack->peek()->extend(temp);
			subStack->push(etemp);
			break;
		}
		
		case '(':	//	Start of a sub-expression
		{
			ExpHead *etemp=new ExpHead(ExpHead::EXP_SIMPLE);
			
			temp->type=Token::TOK_SUB;
			temp->value.subExp=etemp;
			subStack->peek()->extend(temp);
			subStack->push(etemp);
			break;
		}
		
		case ')':	//	End of a sub-expression
			do
			{
				subStack->pop();
			}
			while(subStack->peek()->getType()==ExpHead::EXP_FUNCTION);
	}
}

Expression::Expression()
{
	type=FTYPE_UNKNOWN;
	expression=NULL;
}

Expression::Expression(FunctionType ft)
{
	type=ft;
	expression=NULL;
}

Expression::~Expression()
{
	if(expression!=NULL)
		delete expression;
}

//	Stack operations for conversion to postfix
void Expression::addToken(char token[], int type)
{
	if(cStack->count()==0)
		cStack->push('#');
	
	switch(type)
	{
		case tokNumber:
		case tokVariable:
		case tokFunction:
			extend(token, type);
			break;
		
		/*	Check operator precedence and push into stack or
			pop from stack and extend expression				*/
		case tokOperator:
			if(preceeds(token[0], cStack->peek()))
				cStack->push(token[0]);
			else
			{
				char temp[]="\0\0";
				do
				{
					temp[0]=cStack->pop();
					extend(temp, type);
				}
				while(!preceeds(token[0], cStack->peek()));
				cStack->push(token[0]);
			}
			break;
		case '(':
			cStack->push('#');
			extend(token, type);
			break;
		case ')':
		case ';':
			char temp[]="\0\0";
			while((temp[0]=cStack->pop())!='#')
				extend(temp, tokOperator);
			if (type==')')
				extend(token, type);
	}
}

/*	Evaluate the expression using parameter values in val[]
	Index values are given by Token::VAR_TYPE (ExpHead.cpp)		*/
double Expression::evaluate(double val[])
{
	if(expression!=NULL)
		return expression->evaluate(val);
	
	return -1;
}

void Expression::display()
{
	if(expression==NULL)
		return;
	
	expression->display();
	cerr<<endl;
}

void Expression::parse(char *str)
{
	int len=strlen(str);
	cerr<<"Parsing "<<str<<endl;
	
	//	Remove spaces
	char temp[len+len];
	strcpy(temp, str);
	for(int i=0, cnt=0, move=0; i<=len; i++) {
		if(temp[i]==' ') {
			move=1;
			cnt++;
			continue;
		}
		
		if(move)
			temp[i-cnt]=temp[i];
	}
	
	if(temp[0]=='-') {
		memmove(temp+1, temp, strlen(temp)+1);
		temp[0]='0';
	}
	
	for(char *c=strstr(temp, "(-"); c!=NULL; c=strstr(temp, "(-")) {
		memmove(c+2, c+1, strlen(c+1)+1);
		*(c+1)='0';
	}
	
	len=strlen(temp);
	temp[len]=';';
	temp[len+1]='\0';
	
	e=this;
	if(expression!=NULL)
		delete expression;
	
	expression=new ExpHead(ExpHead::EXP_SIMPLE);
//	cStack->clear();
//	subStack->clear();
	cStack=new Stack<char>;
	subStack=new Stack<ExpHead*>;
	YY_BUFFER_STATE bufState=yy_scan_string(temp);
	yy_switch_to_buffer(bufState);
	yyparse();
	yy_delete_buffer(bufState);
	delete cStack;
	delete subStack;
}

int Expression::error()
{
	if(expression==NULL)
		return 1;
	return 0;
}

void Expression::setError()
{
	if(expression!=NULL) {
		delete expression;
		expression=NULL;
	}
}

void Expression::functionType(FunctionType ft)
{
	type=ft;
}

Expression::FunctionType Expression::functionType()
{
	return type;
}

/*int main()
{
	Expression e(Expression::FTYPE_3);
	Expression e1(Expression::FTYPE_1);
	double val[]={1, 5, 1, 2, 3, 10};

	e.parse("(-t+10*t)^(1/2)");
	e1.parse("sin(x+y)+cos(z+s) + log(2*s)");
	
	e.display();
	cout<<endl<<e.evaluate(val)<<endl;
	e1.display();
	cout<<endl<<e1.evaluate(val)<<endl;
}
*/
#endif
