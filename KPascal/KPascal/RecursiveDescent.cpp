#include <iostream>
#include "Lexer.h"
#include "SymbolTable.h"

#include <map>
#include <vector>

KPascal::Lexer lexer;
KPascal::Token token;
KPascal::SymbolTable symbol;

std::vector<std::string> temporaryVector;

int GlobalOffset = 0;

void factor();
void expr();
void term();
void mstat();
void stat();
void Varlist();
void Varprodprime();
void PLend();

bool ShoveTokenIntoSymbolTable()
{
	return false;
}

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
	if (token.value == "begin") 
	{
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
		for each (std::string myTokenValue in temporaryVector)
		{
			if (symbol.Table.find(myTokenValue) == symbol.Table.end())
			{
				if (token.value == "boolean")
				{
					symbol.Table[myTokenValue].size = 1;
				}
				else if (token.value == "integer")
				{
					symbol.Table[myTokenValue].size = 4;
				}
				symbol.Table[myTokenValue].isMethod = false;
				symbol.Table[myTokenValue].type = token.value;
				symbol.Table[myTokenValue].offset = GlobalOffset;
				GlobalOffset += symbol.Table[myTokenValue].size;
			}
			else
			{
				std::cout << "It seems that you have already defined " << myTokenValue << ". Please try again." << std::endl;
			}
		}
		temporaryVector.clear();
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
				lexer.getToken(token);
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
			//we have a variable 
			temporaryVector.push_back(token.value);
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
		//we have a variable 
		temporaryVector.push_back(token.value);
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
			temporaryVector.push_back(token.value);
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
		temporaryVector.push_back(token.value);
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
			//we have a variable 
			temporaryVector.push_back(token.value);
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
		//we have a variable 
		temporaryVector.push_back(token.value);
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
			//we have a variable 
			temporaryVector.push_back(token.value);
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
		//we have a variable 
		temporaryVector.push_back(token.value);
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
		//we have a variable 
		temporaryVector.push_back(token.value);
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
					//lexer.getToken(token);
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
	temporaryVector.push_back(token.value);
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