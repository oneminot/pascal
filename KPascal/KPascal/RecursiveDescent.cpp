#include <iostream>
#include "Lexer.h"

#include <map>

KPascal::Lexer lexer;
KPascal::Token token;

const int size = 11;
char tokenarray[] = "BiV=VtiV=VtVCIeVCIeVCIE.";
int tokenloc = 0;
int numberoftokens = 1;
void factor();
void expr();
void term();
void mstat();
void stat();

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
	if (token.value == "Begin")
	{
		tokenloc++;
		lexer.getToken(token);
		mstat();
		if (token.value == "End")
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

void Varprodprime()
{
	//to be continued
}

void Varlist()
{
	lexer.getToken(token);
	if (token.value == ";")
	{
		lexer.getToken(token);
		if (!token.isKeyword)
		{
			//we have a variable 
			Varlist();
		}
	}
	else
	{
		//do nothing 
	}
}

void Vari()
{
	lexer.getToken(token);
	if (!token.isKeyword)
	{
		//we have a variable 
		Varlist(); 
		lexer.getToken(token);
		if (token.value == ";")
		{
			Varprodprime();
		}
	}
}

void Localvar()
{

}

void Plist()
{

}

void Proc()
{
	lexer.getToken(token);
	//we have a variable;
	lexer.getToken(token);
	//we should get a left parenthesis now 
	if (token.value == "(")
	{
		Plist();
		lexer.getToken(token);
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
					//success 
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

void Func()
{

}

void PFV()
{
	lexer.getToken(token);
	if (token.value == "var")
	{
		Vari();
	}
	else if (token.value == "procedure")
	{
		Proc();
	}
	else if (token.value == "function")
	{
		Func();
	}
	else
	{
		//do nothing 
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
			//this is where var should go 
			PFV();
			//this is Begin;
			lexer.getToken(token);
			block();
			if (token.value == ".")
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
}

void main()
{
	lexer.getToken(token);
	program();
	std::cout << "Good code" << std::endl;
	system("pause");
}