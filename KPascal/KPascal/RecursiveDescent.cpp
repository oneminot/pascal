#include <iostream>
#include "Lexer.h"
#include "SymbolTable.h"

#include <map>
#include <vector>

KPascal::Lexer lexer;
KPascal::Token token;
KPascal::SymbolTable symbol;

std::vector<std::string> temporaryVector;

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
		lexer.getToken(token);
		factor();
		factorprime();
	}
}

void factor()
{

	if (token.value == "(")
	{
		lexer.getToken(token);
		expr();
		if (token.value == ")")
		{
			lexer.getToken(token);
			factorprime();
		}
		else
		{
			HasError();
		}
	}
	else if (token.sType == "real" || (token.sType == "word" && !token.isKeyword) || token.sType == "integer")
	{
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
		lexer.getToken(token);
		expr();
	}
	else if (token.value == "<")
	{
		lexer.getToken(token);
		expr();
	}
	else if (token.value == ">")
	{
		lexer.getToken(token);
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
	if (token.value == "else")
	{
		lexer.getToken(token);
		stat();
	}
}

void stat()
{
	// looking for a variable 
	if (token.sType == "word" && !token.isKeyword)
	{
		lexer.getToken(token);
		if (token.value == ":=")
		{
			lexer.getToken(token);
			expr();
		}
		else
		{
			HasError();
		}
	}
	else if (token.value == "begin")
	{
		lexer.getToken(token);
		mstat();
		if (token.value == "end")
		{
			lexer.getToken(token);
		}
		else
		{
			HasError();
		}
	}
	else if (token.value == "if")
	{
		lexer.getToken(token);
		bexpr();
		if (token.value == "then")
		{
			lexer.getToken(token);
			stat();
			statprime();
		}
	}
}

void mstatprime()
{
	if (token.value == ";")
	{
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
	if (token.value == "begin") {
		lexer.getToken(token);
		mstat();
		if (token.value == "end")
		{
			lexer.getToken(token);
		}
		else { HasError(); }
	}
	else { HasError(); }
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
	if (token.sType == "word" && !token.isKeyword)
	{
		lexer.getToken(token);
		Varlist();
		//lexer.getToken(token);
		if (token.value == ":")
		{
			lexer.getToken(token);
			Datatype();
			if (token.value == ";")
			{
				Varprodprime();
			}
			else { HasError(); }
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
	else if (!token.isKeyword && token.sType == "word")
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
		if (symbol.Table.find(token.value) == symbol.Table.end())
		{
			std::cout << "You found waldo" << std::endl;

		}
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
	if (!token.isKeyword && token.sType == "word")
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

// function abs(num:integer):integer;
void Func()
{
	//we have a variable;
	lexer.getToken(token);
	//we have a left parenthesis 
	if (token.value == "(")
	{
		lexer.getToken(token);
		Plist();
		if (token.value == ")")
		{
			lexer.getToken(token);
			if (token.value == ":")
			{
				lexer.getToken(token);
				Datatype();
				if (token.value == ";")
				{
					lexer.getToken(token);
					Localvar();
					lexer.getToken(token);
					block();
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
				lexer.getToken(token);
			}
			else { HasError(); }
		}
		else { HasError(); }
	}
	else { HasError(); }
}

void main()
{
	lexer.getToken(token);
	program();
	std::cout << "Good code" << std::endl;
	system("pause");
}