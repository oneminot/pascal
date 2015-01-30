#include <iostream>
#include "Lexer.h"

KPascal::Lexer lexer;
KPascal::Token token;
int index = 0;
void Term();
void Expression();
void Factor();

void FactorPrime()
{
	if (token.value == "*" || token.value == "/")
	{
		lexer.getToken(token);
		Factor();
	}
}

void Factor()
{
	if (token.value == "(")
	{
		lexer.getToken(token);
		Expression();
		if (token.value == ")")
		{
				lexer.getToken(token);
				FactorPrime();
		}
		else
		{
			std::cout << "Error" << std::endl;
			system("pause");
			exit(1);
		}
	}
	else if (token.sType == "integer" || token.sType == "real" || token.sType == "variable")
	{
		lexer.getToken(token);
		FactorPrime();
	}
	else if (token.value == "+" || token.value == "-")
	{
		lexer.getToken(token);
		Term();
		lexer.getToken(token);
		FactorPrime();
	}
	else
	{
		//if (token.value != "ÿ")
		//{
			std::cout << "Error" << std::endl;
			system("pause");
			exit(1);
		//}
	}
}

void TermPrime()
{
	if (token.value == "+" || token.value == "-")
	{
		lexer.getToken(token);
		Term();
		TermPrime();
	}
}

void Term()
{
	Factor();
	TermPrime();
}

void Expression()
{
	Term();
}

void Block()
{
	//needs more work
}

void RescursiveDescentMain()
{
	lexer.getToken(token);
	Block();
}

int main()
{
	int नेपाली = 0;
	RescursiveDescentMain();
	if (token.value != "ÿ")
	{
		std::cout << "Error" << std::endl;
		system("pause");
		exit(1);
	}
	std::cout << "Good code!" << std::endl;
	system("pause");
	return 0;
}