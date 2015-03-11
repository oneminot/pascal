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

		void Datatype(bool IsGlobalVariable = false, std::string MethodName = "", bool IsReturnValue = false, bool IsPassedByReference = false)
		{
			if (token.value == "boolean" || token.value == "integer")
			{
				if (IsGlobalVariable)
				{
					for (auto myTokenValue : temporaryVector)
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
				else if (MethodName != "" && IsReturnValue == false)
				{
					//This is where we handle the parameters of the method 
					for (auto myTokenValue : temporaryVector)
					{
						if (symbol.Table[MethodName].parameters.find(myTokenValue) == symbol.Table[MethodName].parameters.end())
						{
							if (token.value == "boolean")
							{
								symbol.Table[MethodName].parameters[myTokenValue].size = 1;
							}
							if (token.value == "integer")
							{
								symbol.Table[MethodName].parameters[myTokenValue].size = 4;
							}
							symbol.Table[MethodName].parameters[myTokenValue].type = token.value;
							symbol.Table[MethodName].parameters[myTokenValue].offset = symbol.Table[MethodName].size;
							symbol.Table[MethodName].parameters[myTokenValue].isPassedByReference = IsPassedByReference;
							symbol.Table[MethodName].size += symbol.Table[MethodName].parameters[myTokenValue].size;;
						}
					}
					temporaryVector.clear();
					lexer.getToken(token);
				}
				else if (MethodName != "")
				{
					//This is where we get the return type of the method 
					//We should only get here from Func() never from Proc() 
					symbol.Table[MethodName].isMethod = true;
					symbol.Table[MethodName].type = token.value;
					lexer.getToken(token);
				}
			}
			else { HasError(token.value); }
		}

		void Varprod(std::string MethodName = "", bool IsPassedByReference = false)
		{
			if (token.sType == "word" && !token.isKeyword)
			{
				lexer.getToken(token);
				Varlist();
				//lexer.getToken(token);
				if (token.value == ":")
				{
					lexer.getToken(token);
					Datatype(false, MethodName, false, IsPassedByReference);
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

		void Localvar(std::string MethodName = "", bool IsPassedByReference = false)
		{
			if (token.value == "var")
			{
				lexer.getToken(token);
				Varprod(MethodName, IsPassedByReference);
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

		void Plist(std::string MethodName = "")
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
						Datatype(false, MethodName);
						PLend();
					}
				}
			}
			else if (!token.isKeyword && MethodName != "")
			{
				//we have a local variable 
				temporaryVector.push_back(token.value);
				lexer.getToken(token);
				Varlist();
				if (token.value == ":")
				{
					lexer.getToken(token);
					Datatype(false, MethodName);
					PLend();
				}
			}
		}

		void Proc()
		{
			//we have a variable;
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
				HasError(myTokenValue);
			}
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
				HasError(myTokenValue);
			}
			lexer.getToken(token);
			//we have a left parenthesis 
			if (token.value == "(")
			{
				lexer.getToken(token);
				Plist(myTokenValue);
				if (token.value == ")")
				{
					lexer.getToken(token);
					if (token.value == ":")
					{
						lexer.getToken(token);
						Datatype(false, myTokenValue, true);
						if (token.value == ";")
						{
							lexer.getToken(token);
							Localvar(myTokenValue);
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
