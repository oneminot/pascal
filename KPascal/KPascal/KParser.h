#include <map>
#include <vector>
#include <iostream>
#include "Lexer.h"
#include "SymbolTable.h"
#ifndef KParser_H_
#define KParser_H_ 
namespace KPascal
{
	class KParser
	{
	public:
		KPascal::Lexer lexer;
		KPascal::Token token;
		KPascal::SymbolTable symbol;

		std::vector<std::string> temporaryVector;

		int GlobalOffset = 0;
		//bool isGlobalVariable = false;

		bool ShoveTokenIntoSymbolTable()
		{
			return false;
		}

		void HasError(std::string FailingTokenValue = "")
		{
			if (FailingTokenValue == "")
			{
				std::cout << "Error in who knows what method with who knows what token" << std::endl;

			}
			else 
			{
				std::cout << "Error due to " << FailingTokenValue << std::endl;
			}
			std::cout << "Press ENTER to quit." << std::endl;
			std::cin.get();
			exit(1);
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
					HasError(token.value);
				}
			}
			else if (token.sType == "real" || (token.sType == "word" && !token.isKeyword) || token.sType == "integer")
			{
				lexer.getToken(token);
				factorprime();
			}
			else
			{
				HasError(token.value);
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
				HasError(token.value);
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
					HasError(token.value);
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
					HasError(token.value);
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
				else { HasError(token.value); }
			}
			else { HasError(token.value); }
		}

		void Datatype(bool IsGlobalVariable = false)
		{
			if (IsGlobalVariable)
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
				else { HasError(token.value); }
			}
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
					else { HasError(token.value); }
				}
				else { HasError(token.value); }
			}
			else { HasError(token.value); }
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
						else { HasError(token.value); }
					}
					else { HasError(token.value); }
				}
				else { HasError(token.value); }
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
				else { HasError(token.value); }
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
					Datatype(true);
					if (token.value == ";")
					{
						lexer.getToken(token);
						Varprodprime();
					}
					else { HasError(token.value); }
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
			else { HasError(token.value); }
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
							else { HasError(token.value); }
						}
						else { HasError(token.value); }
					}
					else { HasError(token.value); }
				}
				else { HasError(token.value); }
			}
			else { HasError(token.value); }
		}

		// function abs(num:integer):integer;
		void Func()
		{
			std::string myTokenValue = token.value;
			if (symbol.Table.find(myTokenValue) == symbol.Table.end())
			{
				// we can replace the size by the size of the return value later 
				symbol.Table[myTokenValue].size = 0;
				symbol.Table[myTokenValue].isMethod = false;
				symbol.Table[myTokenValue].type = token.value;
				symbol.Table[myTokenValue].offset = GlobalOffset;
				GlobalOffset += symbol.Table[myTokenValue].size;
			}
			else
			{
				std::cout << "It seems that you have already defined " << myTokenValue << ". Please try again." << std::endl;
				HasError();
			}
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
							else { HasError(token.value); }
						}
						else { HasError(token.value); }
					}
					else { HasError(token.value); }
				}
				else { HasError(token.value); }
			}
			else { HasError(token.value); }
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
					else { HasError(token.value); }
				}
				else { HasError(token.value); }
			}
			else { HasError(token.value); }
		}
	};
}
#endif // !KParser_H_
