#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "SymbolTable.h"
#include "KRegisterArray.h"
#include "KArrayDimension.h"
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
		KPascal::KRegisterArray registerArray;
		std::vector<std::string> temporaryVector;

		int m_if_counter = 0;
		int m_while_counter = 0;

		std::ofstream fout;
		int GlobalOffset = 0;
		bool NewRegister = true;

		void HasError(std::string FailingTokenValue = "")
		{
			if (FailingTokenValue == "") { std::cout << "Error in who knows what method with who knows what token" << std::endl; }
			else { std::cout << "Error due to " << FailingTokenValue << std::endl; }
			std::cout << "Press ENTER to quit." << std::endl;
			std::cin.get();
			exit(1);
		}

		void HasError(Token token)
		{
			if (token.value == "") { std::cout << "Error in who knows what method with who knows what token" << std::endl; }
			else { std::cout << "Error due to " << token.value << "of type " << token.sType << std::endl; }
			std::cout << "Press ENTER to quit." << std::endl;
			std::cin.get();
			exit(1);
		}

		void PushAllRegisters()
		{
			for (auto x = 0; x < 6; x++)
			{
				fout << "		push " << registerArray.kRegisters[x].RegisterName << std::endl;
			}
			fout << "		push esp" << std::endl;
			fout << "		push ebp" << std::endl;
			fout << "		lea eax, DataSegment" << std::endl;
			fout << "		mov ebp, eax" << std::endl;
		}

		void PopAllRegisters()
		{
			fout << "		pop ebp" << std::endl;
			fout << "		pop esp" << std::endl;
			for (int x = 5; x > -1; x--)
			{
				fout << "		pop " << registerArray.kRegisters[x].RegisterName << std::endl;
			}
		}

		void MoveRegisterValueToStack(std::string MethodName, Token LeftSideToken, bool IsArray)
		{
			Expression(MethodName);
			NewRegister = true;
			registerArray.currentRegisterIndex--;
			if (IsArray)
			{
				fout << "		mov [ebp + " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << "], " << registerArray.kRegisters[registerArray.currentRegisterIndex].RegisterName << std::endl;
			}
			else 
			{
				fout << "		mov [ebp + " << symbol.Table[LeftSideToken.value].offset << "], " << registerArray.kRegisters[registerArray.currentRegisterIndex].RegisterName << std::endl;
			}
			registerArray.kRegisters[registerArray.currentRegisterIndex].IsUsed = false;
		}

		std::string SymbolToString(std::string symbol)
		{
			std::string return_string;
			if (symbol == "+")
			{
				return_string = "add";
			}
			else if (symbol == "-")
			{
				return_string = "add";
			}
			return return_string;
		}

		std::string FactorPrime(std::string MethodName = "")
		{
			std::string LeftSide;
			std::string RightSide;
			if (token.value == "*")
			{
				lexer.getToken(token);
				LeftSide = Factor(MethodName);
				RightSide = FactorPrime(MethodName);
				if (LeftSide != " ")
				{
					fout << "		mov " << registerArray.kRegisters[registerArray.currentRegisterIndex].RegisterName << ", " << LeftSide << std::endl;
					registerArray.kRegisters[registerArray.currentRegisterIndex].IsUsed = true;
					registerArray.currentRegisterIndex++;
					NewRegister = false;
					if (RightSide != " ")
					{
						return " ";
					}
				}
				return "*";
			}
			else
			{
				return " ";
			}
		}

		std::string Factor(std::string MethodName = "")
		{
			std::string LeftSide = "";
			std::string RightSide = "";
			if (token.value == "(")
			{
				NewRegister = true;
				lexer.getToken(token);
				LeftSide = Expression();
				if (token.value == ")")
				{
					NewRegister = true;
					lexer.getToken(token);
					RightSide = FactorPrime(MethodName);
					if (RightSide == " " && LeftSide != " ")
					{
						std::cout << "we found love" << std::endl;
						fout << "		mov " << registerArray.kRegisters[registerArray.currentRegisterIndex].RegisterName << ", " << LeftSide << std::endl;
						registerArray.kRegisters[registerArray.currentRegisterIndex].IsUsed = true;
						registerArray.currentRegisterIndex++;
						NewRegister = false;
						return " ";
					}
					else if (RightSide == "*" && LeftSide == " ")
					{
						fout << "		imul " << registerArray.kRegisters[registerArray.currentRegisterIndex - 2].RegisterName << ", " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
						registerArray.kRegisters[registerArray.currentRegisterIndex - 1].IsUsed = false;
						registerArray.currentRegisterIndex--;
						return " ";
					}
				}
				else { HasError(token); }
			}
			else if (token.sType == "real" || token.sType == "integer")
			{
				auto ReturnString = token.value;
				lexer.getToken(token);
				LeftSide = FactorPrime(MethodName);
				if (LeftSide == " ")
				{
					return ReturnString;
				}
				else if (LeftSide == "*")
				{
					fout << "		imul " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << ReturnString << std::endl;
					return " ";
				}
			}
			else if (token.sType == "word" && !token.isKeyword)
			{
				Token ReturnToken = token;
				std::string ReturnString = " ";
				// we only have global variables so far so this will be good enough 
				if (symbol.Table.find(ReturnToken.value) != symbol.Table.end())
				{
					ReturnString = "[ebp + " + std::to_string(symbol.Table[ReturnToken.value].offset) + "]";
				}
				lexer.getToken(token);
				LeftSide = FactorPrime(MethodName);
				if (LeftSide == " ") { return ReturnString; }
				else if (LeftSide == "*")
				{
					fout << "		imul " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << ReturnString << std::endl;
					return " ";
				}
			}
			else { HasError(token); }
			return " ";
		}

		std::string TermPrime(std::string MethodName = "")
		{
			std::string LeftSide;
			std::string RightSide;
			std::string ReturnString;
			if (token.value == "+" || token.value == "-")
			{
				ReturnString = token.value;
				lexer.getToken(token);
				LeftSide = Term(MethodName);
				RightSide = TermPrime(MethodName);
				if (NewRegister && LeftSide != " ")
				{
					fout << "		mov " << registerArray.kRegisters[registerArray.currentRegisterIndex].RegisterName << ", ";
					fout << LeftSide << std::endl;
					registerArray.kRegisters[registerArray.currentRegisterIndex].IsUsed = true;
					registerArray.currentRegisterIndex++;
					NewRegister = false;
					return ReturnString;
				}
				//else if (!NewRegister && LeftSide != " " && RightSide == " ")
				//{
				//	fout << "		" << SymbolToString(RightSide) << " ";
				//	fout << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << LeftSide << std::endl;
				//	registerArray.kRegisters[registerArray.currentRegisterIndex - 1].IsUsed = false;
				//	registerArray.currentRegisterIndex--;
				//	return " ";
				//}
				else
				{
					return ReturnString;
				}
			}
			return " ";
		}

		std::string Term(std::string MethodName = "")
		{
			std::string LeftSide;
			std::string RightSide;
			LeftSide = Factor(MethodName);
			CheckArray(MethodName, token);
			// if term prime goes to epsilon, do something 
			if (LeftSide == " ")
			{
				NewRegister = true;
			}
			RightSide = TermPrime(MethodName);
			if (NewRegister && RightSide == " " && LeftSide != " ")
			{
				// add this token to the next available register 
				fout << "		mov " << registerArray.kRegisters[registerArray.currentRegisterIndex].RegisterName << ", " << LeftSide << std::endl;
				registerArray.kRegisters[registerArray.currentRegisterIndex].IsUsed = true;
				registerArray.currentRegisterIndex++;
				NewRegister = false;
				return " ";
			}
			else if ((RightSide == "+" || RightSide == "-") && LeftSide != " ")
			{
				// add this token to the next available register 
				if (RightSide == "-")
				{
					fout << "		neg " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
				}
				fout << "		" << SymbolToString(RightSide) << " ";
				fout << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << LeftSide << std::endl;
				return " ";
			}
			else if ((RightSide == "+" || RightSide == "-") && LeftSide == " ")
			{
				fout << "		neg " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
				fout << "		" << SymbolToString(RightSide) << " ";
				fout << registerArray.kRegisters[registerArray.currentRegisterIndex - 2].RegisterName << ", " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
				registerArray.kRegisters[registerArray.currentRegisterIndex - 1].IsUsed = false;
				registerArray.currentRegisterIndex--;
				return " ";
			}
			return LeftSide;
		}

		std::string Expression(std::string MethodName = "")
		{
			std::string result_string = Term(MethodName);
			return result_string;
		}

		std::string BooleanExpressionPrime(std::string MethodName = "", int local_if_counter = 0, std::string k_loop_type = "")
		{
			//TODO: Add logic for returning a string from boolean prime 
			if (token.value == "=")
			{
				NewRegister = true;
				auto right_side_string = lexer.getToken(token);
				Expression(MethodName);
				fout << "		cmp " << registerArray.kRegisters[registerArray.currentRegisterIndex - 2].RegisterName << ", " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
				registerArray.kRegisters[registerArray.currentRegisterIndex - 2].IsUsed = false;
				registerArray.kRegisters[registerArray.currentRegisterIndex - 1].IsUsed = false;
				registerArray.currentRegisterIndex--;
				registerArray.currentRegisterIndex--;
				if (k_loop_type == "if")
				{
					fout << "		jne  endorelse" << local_if_counter << std::endl;
				}
				if (k_loop_type == "while")
				{
					fout << "		jne  endwhile" << local_if_counter << std::endl;
				}
			}
			else if (token.value == "<")
			{
				NewRegister = true;
				auto right_side_string = lexer.getToken(token);
				Expression(MethodName);
				fout << "		cmp " << registerArray.kRegisters[registerArray.currentRegisterIndex - 2].RegisterName << ", " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
				registerArray.kRegisters[registerArray.currentRegisterIndex - 2].IsUsed = false;
				registerArray.kRegisters[registerArray.currentRegisterIndex - 1].IsUsed = false;
				registerArray.currentRegisterIndex--;
				registerArray.currentRegisterIndex--;
				if (k_loop_type == "if")
				{
					fout << "		jge  endorelse" << local_if_counter << std::endl;
				}
				if (k_loop_type == "while")
				{
					fout << "		jge  endwhile" << local_if_counter << std::endl;
				}
			}
			else if (token.value == ">")
			{
				NewRegister = true;
				auto right_side_string = lexer.getToken(token);
				Expression(MethodName);
				fout << "		cmp " << registerArray.kRegisters[registerArray.currentRegisterIndex - 2].RegisterName << ", " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
				registerArray.kRegisters[registerArray.currentRegisterIndex - 2].IsUsed = false;
				registerArray.kRegisters[registerArray.currentRegisterIndex - 1].IsUsed = false;
				registerArray.currentRegisterIndex--;
				registerArray.currentRegisterIndex--;
				if (k_loop_type == "if")
				{
					fout << "		jle  endorelse" << local_if_counter << std::endl;
				}
				if (k_loop_type == "while")
				{
					fout << "		jle  endwhile" << local_if_counter << std::endl;
				}
			}
			else { HasError(token); }
			return " ";
		}

		std::string BooleanExpression(std::string MethodName = "", int local_if_counter = 0, std::string k_loop_type = "")
		{
			auto LeftSide = Expression(MethodName);
			auto RightSide = BooleanExpressionPrime(MethodName, local_if_counter, k_loop_type);
			return " ";
		}

		std::string StatementPrime(std::string MethodName = "")
		{
			//TODO: Add logic for returning a string from boolean prime 
			if (token.value == "else")
			{
				lexer.getToken(token);
				Statement(MethodName);
			}
			return " ";
		}

		void Statement(std::string MethodName = "")
		{
			// looking for a variable 
			if (token.sType == "word" && !token.isKeyword)
			{
				//make sure the variable is in the symbol table 
				auto IsVariableInParameterList = MethodName != "" && symbol.Table[MethodName].parameters.find(token.value) != symbol.Table[MethodName].parameters.end();
				auto IsVariableInLocalVariableList = MethodName != "" && symbol.Table[MethodName].localvariables.find(token.value) != symbol.Table[MethodName].localvariables.end();
				// the name of the method should already be in the Global Variable List because it should be in the symbol table 
				auto IsVariableInGlobalVariableList = symbol.Table.find(token.value) != symbol.Table.end();
				if (IsVariableInParameterList || IsVariableInLocalVariableList || IsVariableInGlobalVariableList)
				{
					auto LeftSideToken = token;
					lexer.getToken(token);
					auto is_array = CheckArray(MethodName, LeftSideToken);
					if (token.value == ":=")
					{
						lexer.getToken(token);
						auto is_rightside_an_array_element = false; 
						if (symbol.Table[token.value].my_array_size.size() > 0)
						{
							is_rightside_an_array_element = true;
						}
						else
						{
							is_rightside_an_array_element = false;
						}
						if (token.sType == "word" && !token.isKeyword)
						{
							if (IsVariableInParameterList || IsVariableInLocalVariableList || IsVariableInGlobalVariableList)
							{
								MoveRegisterValueToStack(MethodName, LeftSideToken, is_array);
							}
							else { std::cout << "The compiler could not find a definition for " << token.value << ". " << std::endl; HasError(token); }
						}
						else if (token.sType == "integer")
						{
							MoveRegisterValueToStack(MethodName, LeftSideToken, is_array);

						}
						else if (token.sType == "word" && (token.value == "true" || token.value == "false"))
						{
							MoveRegisterValueToStack(MethodName, LeftSideToken, is_array);
						}
						else if (token.value == "(")
						{
							std::cout << "live is for the living my dear" << std::endl;
							MoveRegisterValueToStack(MethodName, LeftSideToken, is_array);
						}
						else { std::cout << "The compiler could not find a definition for " << token.value << ". " << std::endl; HasError(token); }
					}
					//else if (token.value == "(")
					//{
					//	VariableParameter(MethodName);
					//}
					else { HasError(token); }
				}
				else { std::cout << "The compiler could not find a definition for " << token.value << ". " << std::endl; HasError(token); }
			}
			else if (token.value == "begin")
			{
				NewRegister = true;
				lexer.getToken(token);
				MultipleStatement(MethodName);
				if (token.value == "end")
				{
					lexer.getToken(token);
				}
				else { HasError(token); }
			}
			else if (token.value == "if")
			{
				auto local_if_counter = m_if_counter;
				m_if_counter++;
				lexer.getToken(token);
				BooleanExpression(MethodName, local_if_counter, "if");
				if (token.value == "then")
				{
					NewRegister = true;
					lexer.getToken(token);
					Statement(MethodName);
					fout << "		jmp end" << local_if_counter << std::endl;
					fout << "		endorelse" << local_if_counter << ":" << std::endl;
					StatementPrime(MethodName);
					fout << "		end" << local_if_counter << ":" << std::endl;
				}
			}
			else if (token.value == "while")
			{
				auto local_while_counter = m_while_counter;
				fout << "		while" << local_while_counter << ":" << std::endl;
				m_while_counter++;
				lexer.getToken(token);
				BooleanExpression(MethodName, local_while_counter, "while");
				if (token.value == "do")
				{
					NewRegister = true;
					lexer.getToken(token);
					Statement(MethodName);
					fout << "		jmp while" << local_while_counter << std::endl;
					fout << "		endwhile" << local_while_counter << ":" << std::endl;
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
				else { HasError(token); }
			}
			else { HasError(token); }
		}

		std::string MultipleArray(std::string MethodName, Token ArrayName, int ArrayDimensionCounter)
		{
			if (token.value == ",")
			{
				lexer.getToken(token);
				auto maximum_dimension_value = symbol.Table[ArrayName.value].my_array_size[ArrayDimensionCounter].ends_at();
				auto minimum_dimension_value = symbol.Table[ArrayName.value].my_array_size[ArrayDimensionCounter].starts_at();
				auto this_dimension = std::stoi(token.value);
				if (maximum_dimension_value >= this_dimension)
				{
					Expression(MethodName);
					fout << "		sub " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << minimum_dimension_value << std::endl;
					ArrayDimensionCounter++;
					auto my_multiplication_factor = 1;
					for (size_t i = ArrayDimensionCounter; i < symbol.Table[ArrayName.value].my_array_size.size(); i++)
					{
						auto maximum_multiplier_dimension_value = symbol.Table[ArrayName.value].my_array_size[i].ends_at();
						auto minimum_multiplier_dimension_value = symbol.Table[ArrayName.value].my_array_size[i].starts_at();
						my_multiplication_factor *= (maximum_multiplier_dimension_value - minimum_multiplier_dimension_value + 1);
					}
					if (symbol.Table[ArrayName.value].type == "boolean")
					{
						// do nothing 
					}
					if (symbol.Table[ArrayName.value].type == "integer")
					{
						my_multiplication_factor *= 4;
					}
					fout << "		imul " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << my_multiplication_factor << std::endl;
					fout << "		add " << registerArray.kRegisters[registerArray.currentRegisterIndex - 2].RegisterName << ", " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << std::endl;
					registerArray.kRegisters[registerArray.currentRegisterIndex - 1].IsUsed = false;
					registerArray.currentRegisterIndex--;
					NewRegister = true;
					MultipleArray(MethodName, ArrayName, ArrayDimensionCounter);
				}
				else { HasError(token); }
				//std::cin.get();
			}
			return "";
		}

		bool CheckArray(std::string MethodName, Token ArrayName)
		{
			auto ArrayDimensionCounter = 0;
			auto first_dimension = 0;
			if (token.value == "[")
			{
				lexer.getToken(token);
				auto maximum_dimension_value = symbol.Table[ArrayName.value].my_array_size[ArrayDimensionCounter].ends_at();
				auto minimum_dimension_value = symbol.Table[ArrayName.value].my_array_size[ArrayDimensionCounter].starts_at();
				if (token.sType == "integer")
				{
					first_dimension = std::stoi(token.value);
				}
				else if (token.sType == "word")
				{
					auto IsVariableInParameterList = MethodName != "" && symbol.Table[MethodName].parameters.find(token.value) != symbol.Table[MethodName].parameters.end();
					auto IsVariableInLocalVariableList = MethodName != "" && symbol.Table[MethodName].localvariables.find(token.value) != symbol.Table[MethodName].localvariables.end();
					auto IsVariableInGlobalVariableList = symbol.Table.find(token.value) != symbol.Table.end();
					if (IsVariableInGlobalVariableList || IsVariableInLocalVariableList || IsVariableInParameterList)
					{
						std::cout << "Sorry. I cannot handle variables as array dimensions yet. I understand " << token.value << " is a valid token. However, I'm afraid I can't do that." << std::endl;
					}
				}
				if (maximum_dimension_value >= first_dimension)
				{
					Expression(MethodName);
					fout << "		sub " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << minimum_dimension_value << std::endl;
					ArrayDimensionCounter++;
					auto my_multiplication_factor = 1;
					for (size_t i = ArrayDimensionCounter; i < symbol.Table[ArrayName.value].my_array_size.size(); i++)
					{
						auto maximum_multiplier_dimension_value = symbol.Table[ArrayName.value].my_array_size[i].ends_at();
						auto minimum_multiplier_dimension_value = symbol.Table[ArrayName.value].my_array_size[i].starts_at();
						my_multiplication_factor *= (maximum_multiplier_dimension_value - minimum_multiplier_dimension_value + 1);
					}
					if (symbol.Table[ArrayName.value].type == "boolean")
					{
						// do nothing 
					}
					if (symbol.Table[ArrayName.value].type == "integer")
					{
						my_multiplication_factor *= 4;
					}
					fout << "		imul " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << my_multiplication_factor << std::endl;
					NewRegister = true;
					MultipleArray(MethodName, ArrayName, ArrayDimensionCounter);
					fout << "		add " << registerArray.kRegisters[registerArray.currentRegisterIndex - 1].RegisterName << ", " << symbol.Table[ArrayName.value].offset << std::endl;
					if (token.value == "]")
					{
						lexer.getToken(token);
						return true;
					}
				}
				else { HasError(token); }
			}
			return false;
		}

		std::string ArraySize(std::string MethodName)
		{
			if (token.value == ",")
			{
				lexer.getToken(token);
				if (token.sType == "integer")
				{
					/*std::cout << "the pirates love arrays" << std::endl;
					std::cin.get();*/
					auto MyArrayStartIndex = std::stoi(token.value);
					lexer.getToken(token);
					if (token.sType == "special" && token.value == "..")
					{
						/*std::cout << "the pirates love integers" << std::endl;
						std::cin.get();*/
						lexer.getToken(token);
						if (token.sType == "integer")
						{
							auto MyArrayEndIndex = std::stoi(token.value);
							auto k_array_dimension = KArrayDimension(MyArrayStartIndex, MyArrayEndIndex);
							for (auto myTokenValue : temporaryVector)
							{
								symbol.Table[myTokenValue].my_array_size.push_back(k_array_dimension);
							}
							lexer.getToken(token);
							ArraySize(MethodName);
						}
					}
				}
			}
			return "";
		}

		int GetArraySize(std::string _MyTokenValue)
		{
			auto size = 1;
			for (auto k_array_dimension : symbol.Table[_MyTokenValue].my_array_size)
			{
				size *= k_array_dimension.get_size();
			}
			return size;
		}

		std::string ArrayDataType(std::string MethodName)
		{
			auto ArrayElementTypeSize = 0;
			auto ReturnString = "";
			if (token.sType == "word" && token.value == "boolean")
			{
				ArrayElementTypeSize = 1;
				//std::cout << "hello, i am an boolean. i don't hate you" << std::endl;
				//std::cin.get();
				lexer.getToken(token);
				ReturnString = "boolean";
			}
			else if (token.sType == "word" && token.value == "integer")
			{
				ArrayElementTypeSize = 4;
				//std::cout << "hello, i am an integer. i don't hate you" << std::endl;
				//std::cin.get();
				lexer.getToken(token);
				ReturnString = "integer";
			}
			else { HasError(token); }
			for (auto _MyTokenValue : temporaryVector)
			{
				symbol.Table[_MyTokenValue].type = ReturnString;
				symbol.Table[_MyTokenValue].size = ArrayElementTypeSize * GetArraySize(_MyTokenValue);
				symbol.Table[_MyTokenValue].offset = GlobalOffset;
				GlobalOffset += symbol.Table[_MyTokenValue].size;
			}
			temporaryVector.clear();
			return ReturnString;
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
			else if (token.value == "array")
			{
				// the next token must be a "["
				// check array here 
				if (IsGlobalVariable && MethodName == "")
				{
					for (auto myTokenValue : temporaryVector)
					{
						if (symbol.Table.find(myTokenValue) == symbol.Table.end())
						{
							symbol.Table[myTokenValue].isMethod = false;
							//symbol.Table[myTokenValue].type = token.value;
							symbol.Table[myTokenValue].offset = GlobalOffset;
							//GlobalOffset += symbol.Table[myTokenValue].size;
						}
						else
						{
							std::cout << "It seems that you have already defined " << myTokenValue << ". Please try again." << std::endl;
						}
					}
				}
				lexer.getToken(token);
				if (token.value == "[")
				{
					lexer.getToken(token);
					if (token.sType == "integer")
					{
						/*std::cout << "the pirates love arrays" << std::endl;
						std::cin.get();*/
						auto MyArrayStartIndex = std::stoi(token.value);
						lexer.getToken(token);
						if (token.sType == "special" && token.value == "..")
						{
							/*std::cout << "the pirates love integers" << std::endl;
							std::cin.get();*/
							lexer.getToken(token);
							if (token.sType == "integer")
							{
								auto MyArrayEndIndex = std::stoi(token.value);
								auto k_array_dimension = KArrayDimension(MyArrayStartIndex, MyArrayEndIndex);
								for (auto myTokenValue :  temporaryVector)
								{
									symbol.Table[myTokenValue].my_array_size.push_back(k_array_dimension);
								}
								lexer.getToken(token);
								ArraySize(MethodName);
								if (token.sType == "special" && token.value == "]")
								{
									lexer.getToken(token);
									if (token.sType == "word" && token.value == "of")
									{
										lexer.getToken(token);
										ArrayDataType(MethodName);
									}
								}
							}
						}
					}
				}
			}
			else { HasError(token); }
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
					else { HasError(token); }
				}
				else { HasError(token); }
			}
			else { HasError(token); }
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
						else { HasError(token); }
					}
					else { HasError(token); }
				}
				else { HasError(token); }
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
				else { HasError(token); }
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
					else { HasError(token); }
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
			else { HasError(token); }
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
					HasError(token);
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
							else { HasError(token); }
						}
						else { HasError(token); }
					}
					else { HasError(token); }
				}
				else { HasError(token); }
			}
			else { HasError(token); }
		}

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
				HasError(token);
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
							else { HasError(token); }
						}
						else { HasError(token); }
					}
					else { HasError(token); }
				}
				else { HasError(token); }
			}
			else { HasError(token); }
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
							if (fout.is_open())
							{
								// PopAllRegisters();
								fout << "	}" << std::endl;
								fout << "	std::cin.get();" << std::endl;
								fout << "	return 0;" << std::endl;
								fout << "}" << std::endl;
							}
						}
						else { HasError(token); }
					}
					else { HasError(token); }
				}
				else { HasError(token); }
			}
			else { std::cout << "Output file is not open. You should never see this error. Please debug." << std::endl; }
		}

		KParser()
		{
			fout.open("..\\Kasm\\main.cpp");
			if (fout.is_open())
			{
				fout << "#include <fstream>" << std::endl;
				fout << "#include <iostream>" << std::endl;
				fout << "char DataSegment[65536];" << std::endl;
				fout << "int main()" << std::endl;
				fout << "{" << std::endl;
				fout << "	_asm" << std::endl;
				fout << "	{" << std::endl;
				// PushAllRegisters();
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
