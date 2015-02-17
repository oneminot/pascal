#include <iostream>
#include "Lexer.h"

#include <map>

KPascal::Lexer lexer;
KPascal::Token token;

const int size = 11;
int tokenloc = 0;
int numberoftokens = 1;
void factor();
void expr();
void term();
void mstat();
void stat();
void Varlist();
void Varprodprime();
void PLend();

void HasError()
{
	std::cout << "Error" << std::endl;
	system("pause");
	exit(0);
}

void factorprime()
{
	if (token.value == "*" || token.value == ")")
	{
		tokenloc++;
		lexer.getToken(token);
		factor();
		factorprime();
	}
}

void factor()
{

	if (token.value == "(")
	{
		tokenloc++;
		lexer.getToken(token);
		expr();
		if (token.value == ")")
		{
			tokenloc++;
			lexer.getToken(token);
			factorprime();
		}
		else
		{
			HasError();
		}
	}
	else if (token.value == "R" || token.value == "V" || token.value == "I")
	{
		tokenloc++;
		lexer.getToken(token);
		factorprime();
	}
	else
	{
		HasError();
	}
}


void termprime()
{
	if (token.value == "+" || token.value == "-")
	{
		tokenloc++;
		lexer.getToken(token);
		term();
		termprime();
	}
}

void term()
{
	factor();
	termprime();
}

void expr()
{
	term();
}

void bexprprime()
{
	if (token.value == "=")
	{
		tokenloc++;
		lexer.getToken(token);
		expr();
	}
	else if (token.value == "<")
	{
		tokenloc++;
		expr();
	}
	else
	{
		HasError();
	}
}

void bexpr()
{
	expr();
	bexprprime();
}

void statprime()
{
	if (token.value == "e")
	{
		tokenloc++;
		lexer.getToken(token);
		stat();
	}
}

void stat()
{
	if (token.value == "V")
	{
		tokenloc++;
		lexer.getToken(token);
		if (token.value == "C")
		{
			tokenloc++;
			lexer.getToken(token);
			expr();
		}
		else
		{
			HasError();
		}
	}
	else if (token.value == "B")
	{
		tokenloc++;
		lexer.getToken(token);
		mstat();
		if (token.value == "E")
		{
			tokenloc++;
			lexer.getToken(token);
		}
		else
		{
			HasError();
		}
	}
	else if (token.value == "i")
	{
		tokenloc++;
		lexer.getToken(token);
		bexpr();
		if (token.value == "t")
		{
			tokenloc++;
			lexer.getToken(token);
			stat();
			statprime();
		}
		else
		{
			HasError();
		}
	}
}

void mstatprime()
{
	if (token.value == ";")
	{
		tokenloc++;
		lexer.getToken(token);
		mstat();
	}
}

void mstat()
{
	stat();
	mstatprime();
}

void block()
{
	if (token.value == "begin")
	{
		tokenloc++;
		lexer.getToken(token);
		mstat();
		if (token.value == "end")
		{
			tokenloc++;
			lexer.getToken(token);
		}
		else
		{
			HasError();
		}
	}
	else
	{
		HasError();
	}
}

void Datatype()
{
	if (token.value == "boolean" || token.value == "integer") 
	{ 
		lexer.getToken(token); 
	}
	else { HasError(); }
}

void Varprod()
{
	Varlist();
	lexer.getToken(token);
	if (token.value == ":")
	{
		Datatype();
		lexer.getToken(token);
		if (token.value == ";")
		{
			Varprodprime();
		}
		else { HasError(); }
	}
	else { HasError(); }
}

void Varprodprime()
{
	if (token.value == "var")
	{
		lexer.getToken(token);
		if (!token.isKeyword)
		{
			lexer.getToken(token);
			Varlist();
			if (token.value == ":")
			{
				lexer.getToken(token);
				Datatype();
				if (token.value == ";")
				{
					lexer.getToken(token);
					Varprodprime();
				}
				else { HasError(); }
			}
			else { HasError(); }
		}
		else { HasError(); }
	}
	else
	{
		if (!token.isKeyword)
		{
			lexer.getToken(token);
			Varlist();
			if (token.value == ":")
			{
				lexer.getToken(token);
				Datatype();
				if (token.value == ";")
				{
					lexer.getToken(token);
					Varprodprime();
				}
			}
			else { HasError(); }
		}
	}
}

void Varlist()
{
	if (token.value == ",")
	{
		lexer.getToken(token);
		if (!token.isKeyword)
		{
			//we have a variable 
			lexer.getToken(token);
			Varlist();
		}
	}
}

void Vari()
{
	if (!token.isKeyword)
	{
		//we have a variable 
		lexer.getToken(token);
		Varlist();
		if (token.value == ":")
		{
			lexer.getToken(token);
			Datatype();
			if (token.value == ";")
			{
				lexer.getToken(token);
				Varprodprime();
			}
			else { HasError(); }
		}
	}
}

void Localvar()
{
	lexer.getToken(token);
	if (token.value == "var")
	{
		lexer.getToken(token);
		Varprod();
	}
}

void PLprime()
{
	if (token.value == "var")
	{
		lexer.getToken(token);
		if (!token.isKeyword)
		{
			lexer.getToken(token);
			Varlist();
			if (token.value == ":")
			{
				lexer.getToken(token);
				Datatype();
				PLend();
			}
		}
	}
	else if (!token.isKeyword)
	{
		lexer.getToken(token);
		Varlist();
		if (token.value == ":")
		{
			lexer.getToken(token);
			Datatype();
			PLend();
		}
	}
	else { HasError(); }
}

void PLend()
{
	if (token.value == ";")
	{
		lexer.getToken(token);
		PLprime();
	}
}

void Plist()
{
	if (token.value == "var")
	{
		lexer.getToken(token);
		if (!token.isKeyword)
		{
			lexer.getToken(token);
			Varlist();
			if (token.value == ":")
			{
				lexer.getToken(token);
				Datatype();
				PLend();
			}
		}
	}
	else if (!token.isKeyword)
	{
		lexer.getToken(token);
		Varlist();
		if (token.value == ":")
		{
			lexer.getToken(token);
			Datatype();
			PLend();
		}
	}
}

void Proc()
{
	//we have a variable;
	if (!token.isKeyword)
	{
		lexer.getToken(token);
		//we should get a left parenthesis now 
		if (token.value == "(")
		{
			lexer.getToken(token);
			Plist();
			//we should have a right parenthesis now
			if (token.value == ")")
			{
				lexer.getToken(token);
				if (token.value == ";")
				{
					lexer.getToken(token);
					Localvar();
					block();
					lexer.getToken(token);
					if (token.value == ";")
					{
						lexer.getToken(token);
					}
					else { HasError(); }
				}
				else { HasError(); }
			}
			else { HasError(); }
		}
		else { HasError(); }
	}
	else { HasError(); }
}

void Func()
{
	lexer.getToken(token);
	lexer.getToken(token);
	//we have a variable;
	lexer.getToken(token);
	if (token.value == "(")
	{
		Plist();
		lexer.getToken(token);
		if (token.value == ")")
		{
			lexer.getToken(token);
			if (token.value == ":")
			{
				lexer.getToken(token);
				Datatype();
				lexer.getToken(token);
				if (token.value == ";")
				{
					lexer.getToken(token);
				}
				else { HasError(); }
			}
			else { HasError(); }
		}
		else { HasError(); }
	}
	else { HasError(); }
}

void PFV()
{
	if (token.value == "var")
	{
		lexer.getToken(token);
		Vari();
		PFV();
	}
	else if (token.value == "procedure")
	{
		lexer.getToken(token);
		Proc();
		PFV();
	}
	else if (token.value == "function")
	{
		lexer.getToken(token);
		Func();
		PFV();
	}
}

void program()
{
	if (token.value == "program")
	{
		//this is the program name 
		lexer.getToken(token);
		//this is the semi colon 
		lexer.getToken(token);
		if (token.value == ";")
		{
			lexer.getToken(token);
			PFV();
			block();
			if (token.value == ".")
			{
				tokenloc++;
				lexer.getToken(token);
			}
			else { HasError(); }
		}
		else { HasError(); }
	}
}

void main()
{
	lexer.getToken(token);
	program();
	std::cout << "Good code" << std::endl;
	system("pause");
}