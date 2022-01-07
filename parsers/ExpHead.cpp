//	Head node for each sub-expression

#ifndef _EXPHEAD
#define _EXPHEAD

#include "Stack.cpp"
#include "function.cpp"

int sqrno;

class ExpHead;

struct Token
{
	//	Token.type
	enum TOK_TYPE {
		TOK_NUM=0,
		TOK_VAR,
		TOK_OP,
		TOK_SUB
	};
	
	//	Token.value.variable
	enum VAR_TYPE {
		VAR_X=0,
		VAR_Y,
		VAR_Z,
		VAR_S,
		VAR_T
	};
	
	TOK_TYPE type;	//	0 - 3
	union
	{
		double number;		//	type = 0
		VAR_TYPE variable;		//	type = 1
		char oper;			//	type = 2
		ExpHead *subExp;	//	type = 3
	}
	value;
	Token *next;
};

//	ExpHead.type
class ExpHead
{
	public:
		enum EXP_TYPE {
			EXP_SIMPLE=0,
			EXP_FUNCTION
		};
	
	private:
		EXP_TYPE type;
		Token *head, *curr;
		double (*call)(double);
		
		double compute(double a, char opr, double b)
		{
			double temp;
			
			switch(opr)
			{
				case '+':
					temp=(double)a+b;
					break;
				case '-':
					temp=(double)a-b;
					break;
				case '*':
					temp=(double)a*b;
					break;
				case '/':
					temp=(double)a/b;
					break;
				case '^':
					temp=(double)pow(a, b);
					break;
			}
			
			return temp;
		}
		
	public:
		ExpHead(EXP_TYPE typ, char *func=NULL)
		{
			type = typ;
			
			if(type==EXP_FUNCTION)
				call=fTable[getFunctionCode(func)].call;
			else
				call=NULL;
			head=curr=NULL;
		}
		
		void extend(Token *tok)
		{
			if(curr==NULL)
				head=curr=tok;
			else
			{
				curr->next=tok;
				curr=tok;
			}
			tok->next=NULL;
		}
		
		~ExpHead()
		{
			if(head==NULL)
				return;
			
			if(type==1)
			{
				delete head->value.subExp;
				delete head;
			}
			else
				for(Token *temp=head, *ptr=temp->next; temp!=NULL; )
				{
					if(temp->type==3)
						delete temp->value.subExp;
					delete temp;
					
					temp=ptr;
					if(temp!=NULL)
						ptr=temp->next;
				}
		}
		
		double evaluate(double *val)
		{
			int i;
			double ans;
			
			if(type==EXP_FUNCTION)
			{
				float temp;
				
				temp=head->value.subExp->evaluate(val);
				ans=call(temp);
			}
			else
			{
				Stack<double> FStack;
				
				for(Token *temp=head; temp!=NULL; temp=temp->next)
				{
					switch(temp->type)
					{
						case Token::TOK_NUM:
							FStack.push(temp->value.number);
							break;
						case Token::TOK_VAR:
							if(temp->value.variable!=Token::VAR_T)
								FStack.push(val[temp->value.variable]);
							else
								FStack.push(*val);
							break;
						case Token::TOK_OP:
							double a, b;
							
							b=FStack.pop();
							a=FStack.pop();
							FStack.push(compute(a, temp->value.oper, b));
							break;
						case Token::TOK_SUB:
							FStack.push(temp->value.subExp->evaluate(val));
					}
				}
				ans=FStack.pop();
			}
			
			return ans;
		}
		
		int getType()
		{
			return type;
		}
		
		void display()
		{
			cerr<<'(';
			if(type==1)
			{
				cerr<<"function";
				head->value.subExp->display();
			}
			else
				for(Token *temp=head; temp!=NULL; temp=temp->next)
				{
					switch(temp->type)
					{
						case Token::TOK_NUM:
							cerr<<temp->value.number;
							break;
						case Token::TOK_VAR:
							switch(temp->value.variable) {
								case Token::VAR_X:
									cerr<<'x';
									break;
								case Token::VAR_Y:
									cerr<<'y';
									break;
								case Token::VAR_Z:
									cerr<<'z';
									break;
								case Token::VAR_S:
									cerr<<'s';
									break;
								case Token::VAR_T:
									cerr<<'t';
									break;
							}
							break;
						case Token::TOK_OP:
							cerr<<temp->value.oper;
							break;
						case Token::TOK_SUB:
							temp->value.subExp->display();
					}
				}
			cerr<<')';
		}
};

#endif
