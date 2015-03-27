#include <map>
#include <vector>
#include <iostream>
#include <fstream>
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

		std::ofstream fout;
		int GlobalOffset = 0;

		void HasError(std::string FailingTokenValue = "")
		{
			if (FailingTokenValue == "") { std::cout << "Error in who knows what method with who knows what token" << std::endl; }
			else { std::cout << "Error due to " << FailingTokenValue << std::endl; }
			std::cout << "Press ENTER to quit." << std::endl;
			std::cin.get();
			exit(1);
		}

		void FactorPrime(std::string MethodName = "")
		{
			if (token.value == "*" || token.value == ")")
			{
				lexer.getToken(token);
				Factor(MethodName);
				FactorPrime(MethodName);
			}
		}

		void Factor(std::string MethodName = "")
		{

			if (token.value == "(")
			{
				lexer.getToken(token);
				Expression();
				if (token.value == ")")
				{
					lexer.getToken(token);
					FactorPrime(MethodName);
				}
				else { HasError(token.value); }
			}
			else if (token.sType == "real" || (token.sType == "word" && !token.isKeyword) || token.sType == "integer")
			{
				lexer.getToken(token);
				FactorPrime(MethodName);
			}
			else { HasError(token.value); }
		}

		void TermPrime(std::string MethodName = "")
		{
			if (token.value == "+" || token.value == "-")
			{
				lexer.getToken(token);
				Term(MethodName);
				TermPrime(MethodName);
			}
		}

		void Term(std::string MethodName = "")
		{
			Factor(MethodName);
			TermPrime(MethodName);
		}

		void Expression(std::string MethodName = "")
		{
			Term(MethodName);
		}

		void BooleanExpressionPrime(std::string MethodName = "")
		{
			if (token.value == "=")
			{
				lexer.getToken(token);
				Expression(MethodName);
			}
			else if (token.value == "<")
			{
				lexer.getToken(token);
				Expression(MethodName);
			}
			else if (token.value == ">")
			{
				lexer.getToken(token);
				Expression(MethodName);
			}
			else { HasError(token.value); }
		}

		void BooleanExpression(std::string MethodName = "")
		{
			Expression(MethodName);
			BooleanExpressionPrime();
		}

		void StatementPrime(std::string MethodName = "")
		{
			if (token.value == "else")
			{
				lexer.getToken(token);
				Statement(MethodName);
			}
		}

		void Statement(std::string MethodName = "")
		{
			// looking for a variable 
			if (token.sType == "word" && !token.isKeyword)
			{
				//make sure the variable is in the symbol table 
				bool IsVariableInParameterList = MethodName != "" && symbol.Table[MethodName].parameters.find(token.value) != symbol.Table[MethodName].parameters.end();
				bool IsVariableInLocalVariableList = MethodName != "" && symbol.Table[MethodName].localvariables.find(token.value) != symbol.Table[MethodName].localvariables.end();
				// the name of the method should already be in the Global Variable List because it should be in the symbol table 
				bool IsVariableInGlobalVariableList = symbol.Table.find(token.value) != symbol.Table.end();
				if (IsVariableInParameterList || IsVariableInLocalVariableList || IsVariableInGlobalVariableList)
				{
					lexer.getToken(token);
					if (token.value == ":=")
					{
						lexer.getToken(token);
						if (token.sType == "word" && !token.isKeyword)
						{
							//make sure the variable is in the symbol table 
							bool IsVariableInParameterList = MethodName != "" && symbol.Table[MethodName].parameters.find(token.value) != symbol.Table[MethodName].parameters.end();
							bool IsVariableInLocalVariableList = MethodName != "" && symbol.Table[MethodName].localvariables.find(token.value) != symbol.Table[MethodName].localvariables.end();
							// the name of the method should already be in the Global Variable List because it should be in the symbol table 
							bool IsVariableInGlobalVariableList = symbol.Table.find(token.value) != symbol.Table.end();
							if (IsVariableInParameterList || IsVariableInLocalVariableList || IsVariableInGlobalVariableList)
							{
								Expression(MethodName);
							}
							else { std::cout << "The compiler could not find a definition for " << token.value << ". " << std::endl; HasError(token.value); }
						}
						else if (token.sType == "integer")
						{
							Expression(MethodName);
						}
						else if (token.sType == "word" && (token.value == "true" || token.value == "false"))
						{
							Expression(MethodName);
						}
						else { std::cout << "The compiler could not find a definition for " << token.value << ". " << std::endl; HasError(token.value); }
					}
					else { HasError(token.value); }
				}
				else { std::cout << "The compiler could not find a definition for " << token.value << ". " << std::endl; HasError(token.value); }
			}
			else if (token.value == "begin")
			{
				lexer.getToken(token);
				MultipleStatement(MethodName);
				if (token.value == "end")
				{
					lexer.getToken(token);
				}
				else { HasError(token.value); }
			}
			else if (token.value == "if")
			{
				lexer.getToken(token);
				BooleanExpression();
				if (token.value == "then")
				{
					lexer.getToken(token);
					Statement(MethodName);
					StatementPrime(MethodName);
				}
			}
		}

		void MultipleStatementPrime(std::string MethodName = "")
		{
			if (token.value == ";")
			{
				lexer.getToken(token);
				MultipleStatement(MethodName);
			}
		}

		void MultipleStatement(std::string MethodName = "")
		{
			Statement(MethodName);
			MultipleStatementPrime(MethodName);
		}

		void Block(std::string MethodName = "")
		{
			if (token.value == "begin")
			{
				lexer.getToken(token);
				MultipleStatement(MethodName);
				if (token.value == "end")
				{
					lexer.getToken(token);
				}
				else { HasError(token.value); }
			}
			else { HasError(token.value); }
		}

		void DataType(bool IsGlobalVariable = false, std::string MethodName = "", bool IsReturnValue = false, bool IsPassedByReference = false, bool IsLocalVariable = false)
		{
			if (token.value == "boolean" || token.value == "integer")
			{
				if (IsGlobalVariable && MethodName == "")
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
				else if (MethodName != "" && IsReturnValue == false && !IsLocalVariable)
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
							// for now we assume that parameters are not methods 
							symbol.Table[MethodName].parameters[myTokenValue].isMethod = false;
							symbol.Table[MethodName].parameters[myTokenValue].offset = symbol.Table[MethodName].size;
							symbol.Table[MethodName].parameters[myTokenValue].isPassedByReference = IsPassedByReference;
							symbol.Table[MethodName].size += symbol.Table[MethodName].parameters[myTokenValue].size;;
						}
					}
					temporaryVector.clear();
					lexer.getToken(token);
				}
				else if (MethodName != "" && IsReturnValue == false && IsLocalVariable)
				{
					//This is where we handle the local variables of the method 
					for (auto myTokenValue : temporaryVector)
					{
						if (symbol.Table[MethodName].parameters.find(myTokenValue) == symbol.Table[MethodName].parameters.end() && symbol.Table[MethodName].localvariables.find(myTokenValue) == symbol.Table[MethodName].localvariables.end())
						{
							if (token.value == "boolean")
							{
								symbol.Table[MethodName].localvariables[myTokenValue].size = 1;
							}
							if (token.value == "integer")
							{
								symbol.Table[MethodName].localvariables[myTokenValue].size = 4;
							}
							symbol.Table[MethodName].localvariables[myTokenValue].type = token.value;
							// for now we assume that parameters are not methods 
							symbol.Table[MethodName].localvariables[myTokenValue].isMethod = false;
							symbol.Table[MethodName].localvariables[myTokenValue].offset = symbol.Table[MethodName].size;
							//symbol.Table[MethodName].localvariables[myTokenValue].isPassedByReference = IsPassedByReference;
							symbol.Table[MethodName].size += symbol.Table[MethodName].localvariables[myTokenValue].size;;
						}
					}
					temporaryVector.clear();
					lexer.getToken(token);
				}
				else if (MethodName != "")
				{
					//This is where we get the return type of the method 
					//We should only get here from Function() never from Procedure() 
					symbol.Table[MethodName].isMethod = true;
					symbol.Table[MethodName].type = token.value;
					lexer.getToken(token);
				}
			}
			else { HasError(token.value); }
		}

		void VariableProduction(bool IsGlobalVariable = false, std::string MethodName = "", bool IsPassedByReference = false, bool IsLocalVariable = false)
		{
			if (token.sType == "word" && !token.isKeyword)
			{
				lexer.getToken(token);
				VariableList();
				//lexer.getToken(token);
				if (token.value == ":")
				{
					lexer.getToken(token);
					DataType(false, MethodName, false, IsPassedByReference, IsLocalVariable);
					if (token.value == ";")
					{
						lexer.getToken(token);
						VariableProductionPrime(IsGlobalVariable, MethodName, IsLocalVariable);
					}
					else { HasError(token.value); }
				}
				else { HasError(token.value); }
			}
			else { HasError(token.value); }
		}

		void VariableProductionPrime(bool IsGlobalVariable = false, std::string MethodName = "", bool IsLocalVariable = false)
		{
			if (token.value == "var")
			{
				lexer.getToken(token);
				if (!token.isKeyword)
				{
					//we have a variable 
					temporaryVector.push_back(token.value);
					lexer.getToken(token);
					VariableList();
					if (token.value == ":")
					{
						lexer.getToken(token);
						DataType(IsGlobalVariable, MethodName, false, false, IsLocalVariable);
						if (token.value == ";")
						{
							lexer.getToken(token);
							VariableProductionPrime(IsGlobalVariable, MethodName, IsLocalVariable);
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
				VariableList();
				if (token.value == ":")
				{
					lexer.getToken(token);
					DataType();
					if (token.value == ";")
					{
						lexer.getToken(token);
						VariableProductionPrime();
					}
				}
				else { HasError(token.value); }
			}
		}

		void VariableList()
		{
			if (token.value == ",")
			{
				lexer.getToken(token);
				if (!token.isKeyword)
				{
					//we have a variable 
					temporaryVector.push_back(token.value);
					lexer.getToken(token);
					VariableList();
				}
			}
		}

		void Variable(bool IsGlobalVariable = false, std::string MethodName = "", bool IsLocalVariable = false)
		{
			if (!token.isKeyword)
			{
				//we have a variable 
				temporaryVector.push_back(token.value);
				lexer.getToken(token);
				VariableList();
				if (token.value == ":")
				{
					lexer.getToken(token);
					DataType(IsGlobalVariable, MethodName, IsLocalVariable);
					if (token.value == ";")
					{
						lexer.getToken(token);
						VariableProductionPrime(IsGlobalVariable, MethodName, IsLocalVariable);
					}
					else { HasError(token.value); }
				}
			}
		}

		void LocalVariable(bool IsGlobalVariable = false, std::string MethodName = "", bool IsPassedByReference = false, bool IsLocalVariable = false)
		{
			if (token.value == "var")
			{
				lexer.getToken(token);
				temporaryVector.push_back(token.value);
				VariableProduction(IsGlobalVariable, MethodName, IsPassedByReference, IsLocalVariable);
			}
		}

		void ParameterListPrime(std::string MethodName = "")
		{
			if (token.value == "var")
			{
				lexer.getToken(token);
				if (!token.isKeyword)
				{
					//we have a variable 
					temporaryVector.push_back(token.value);
					lexer.getToken(token);
					VariableList();
					if (token.value == ":")
					{
						lexer.getToken(token);
						DataType(false, MethodName);
						ParameterListEnd(MethodName);
					}
				}
			}
			else if (!token.isKeyword)
			{
				//we have a variable 
				temporaryVector.push_back(token.value);
				lexer.getToken(token);
				VariableList();
				if (token.value == ":")
				{
					lexer.getToken(token);
					DataType(false, MethodName);
					ParameterListEnd(MethodName);
				}
			}
			else { HasError(token.value); }
		}

		void ParameterListEnd(std::string MethodName = "")
		{
			if (token.value == ";")
			{
				lexer.getToken(token);
				ParameterListPrime(MethodName);
			}
		}

		void ParameterList(std::string MethodName = "")
		{
			if (token.value == "var")
			{
				lexer.getToken(token);
				if (!token.isKeyword)
				{
					//we have a variable 
					temporaryVector.push_back(token.value);
					lexer.getToken(token);
					VariableList();
					if (token.value == ":")
					{
						lexer.getToken(token);
						DataType(false, MethodName, false, true);
						ParameterListEnd(MethodName);
					}
				}
			}
			else if (!token.isKeyword && MethodName != "")
			{
				//we have a local variable 
				temporaryVector.push_back(token.value);
				lexer.getToken(token);
				VariableList();
				if (token.value == ":")
				{
					lexer.getToken(token);
					DataType(false, MethodName);
					ParameterListEnd(MethodName);
				}
			}
		}

		void Procedure()
		{
			if (!token.isKeyword && token.sType == "word")
			{
				std::string myTokenValue = token.value;
				if (symbol.Table.find(myTokenValue) == symbol.Table.end())
				{
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
				//we should get a left parenthesis now 
				if (token.value == "(")
				{
					lexer.getToken(token);
					ParameterList(myTokenValue);
					//we should have a right parenthesis now
					if (token.value == ")")
					{
						lexer.getToken(token);
						if (token.value == ";")
						{
							lexer.getToken(token);
							LocalVariable(false, myTokenValue, false, true);
							Block(myTokenValue);
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
		void Function()
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
				ParameterList(myTokenValue);
				if (token.value == ")")
				{
					lexer.getToken(token);
					if (token.value == ":")
					{
						lexer.getToken(token);
						DataType(false, myTokenValue, true);
						if (token.value == ";")
						{
							lexer.getToken(token);
							LocalVariable(false, myTokenValue, false, true);
							Block(myTokenValue);
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

		void ParameterFunctionVariable(bool IsGlobalVariable = false, std::string MethodName = "")
		{
			if (token.value == "var")
			{
				lexer.getToken(token);
				Variable(IsGlobalVariable, MethodName);
				ParameterFunctionVariable(IsGlobalVariable, MethodName);
			}
			else if (token.value == "procedure")
			{
				lexer.getToken(token);
				Procedure();
				ParameterFunctionVariable(IsGlobalVariable, MethodName);
			}
			else if (token.value == "function")
			{
				lexer.getToken(token);
				Function();
				ParameterFunctionVariable(IsGlobalVariable, MethodName);
			}
		}
		void Program()
		{
			if (fout.is_open())
			{
				if (token.value == "program")
				{
					fout << "kus was here!" << std::endl;
					//this is the program name 
					lexer.getToken(token);
					//this is the semi colon 
					lexer.getToken(token);
					if (token.value == ";")
					{
						lexer.getToken(token);
						ParameterFunctionVariable(true);
						Block();
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
			else { std::cout << "Output file is not open. You should never see this error. Please debug." << std::endl; }
		}

		KParser()
		{
			fout.open("..\\kAssembly.txt");
			if (fout.is_open())
			{
				fout << "lea eax, DataSegment" << std::endl;
				fout << "mov ebp, eax" << std::endl;
			}
			else { std::cout << "Output file is not open." << std::endl; }
		}
		~KParser()
		{
			fout.close();
		}
	};
}
#endif // !KParser_H_
