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
			std::cout << "Error" << std::endl;
			system("pause");
			exit(0);
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
		std::cout << "Error" << std::endl;
		system("pause");
		exit(0);
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
		std::cout << "Error" << std::endl;
		system("pause");
		exit(0);
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
			std::cout << "Error" << std::endl;
			system("pause");
			exit(0);
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
			std::cout << "Error" << std::endl;
			system("pause");
			exit(0);
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
			std::cout << "Error" << std::endl;
			system("pause");
			exit(0);
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
	if (token.value == "B")
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
			std::cout << "Error" << std::endl;
			system("pause");
			exit(0);
		}
	}
	else
	{
		std::cout << "Error" << std::endl;
		system("pause");
		exit(0);
	}
}

void program()
{
	if (token.value == "Program")
	{
		lexer.getToken(token);
		block();
		if (token.value == ".")
		{
			tokenloc++;
			lexer.getToken(token);
		}
		else
		{
			std::cout << "Error" << std::endl;
			system("pause");
			exit(0);
		}
	}
}

void main()
{
	while (true)
	{
		lexer.getToken(token);
	}
	program();
	std::cout << "Good code" << std::endl;
	system("pause");
}