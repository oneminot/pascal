//#ifndef Parser_H_ 
//#define Parser_H_ 
//#include "Keyword.h"
//#include "Lexer.h"
//#include "Token.h"
//#include <vector>
//#include <string>
//namespace KPascal
//{
//	class Parser
//	{
//	public:
//		Parser()
//		{
//			fin.open(tableFileName);
//			if (fin.is_open())
//			{
//				for (size_t i = 0; i < 15; i++)
//				{
//					for (size_t j = 0; j < 15; j++)
//					{
//						fin >> NatArray[i][j];
//					}
//				}
//				Lexer lexer;
//				Token token;
//			}
//			//myStack.push_back("$");
//			myStack.push_back("15");
//		};
//		~Parser(){};
//
//		std::string getTokenTypeString(Token token)
//		{
//			std::string test = "";
//			if (token.type < 10)
//			{
//				test += "0";
//			}
//			test += token.type;
//			return test;
//		}
//
//		bool parse(Token token)
//		{
//			Keyword keyword;
//			int TopOfTheStack = std::atoi(myStack.back().c_str());
//			std::string strIncomingToken = keyword.getKeywordValue(token);
//			int IncomingToken = std::stoi(strIncomingToken);
//			std::string tokenType = getTokenTypeString(token);
//			std::string result;
//			result.push_back(NatArray[TopOfTheStack - 1][IncomingToken - 1]);
//			if (result == ".")
//			{
//				std::cout << "Error" << std::endl;
//				system("pause");
//			}
//			if (result == "<")
//			{
//				myStack.push_back(result);
//				myStack.push_back(strIncomingToken);
//				return true;
//			}
//			if (result == ">")
//			{
//				reduce(token);
//				return true;
//			}
//			if (result == "=")
//			{
//				myStack.push_back(result);
//				myStack.push_back(strIncomingToken);
//				return true;
//			}
//			return false;
//		}
//
//		std::vector<std::string> getmystack()
//		{
//			return myStack;
//		}
//
//		void reduce(Token token)
//		{
//			std::string strReduce = "";
//			std::string result;
//			do
//			{
//				while (true)
//				{
//					std::string back = myStack.back();
//					if (back == "<")
//					{
//						KPascal::Production p;
//						std::string strReduced = p.getProductionValue(strReduce);
//						myStack.pop_back();
//						std::string ReducedTopOfStack = myStack.back();
//						std::string AddToStack;
//						AddToStack.push_back(NatArray[std::stoi(ReducedTopOfStack) - 1][std::stoi(strReduced) - 1]);
//						myStack.push_back(AddToStack);
//						myStack.push_back(strReduced);
//						break;
//					}
//					if (back != "=")
//					{
//						strReduce = back + strReduce;
//					}
//					myStack.pop_back();
//				}
//				strReduce = "";
//				Keyword keyword;
//				int TopOfTheStack = std::atoi(myStack.back().c_str());
//				std::string strIncomingToken = keyword.getKeywordValue(token);
//				int IncomingToken = std::stoi(strIncomingToken);
//				result = "";
//				result.push_back(NatArray[TopOfTheStack - 1][IncomingToken - 1]);
//				if (result == "<" || result == "=") 
//					//break out of the loop so we can put the current driving token on 
//				{
//					break;
//				}
//				if (result == "a")
//				{
//					break;
//				}
//			} while (true);
//			// we put the current token on this happens only if the result is a < or a = 
//			if (result == "a")
//			{
//				return;
//			}
//			myStack.push_back(result);
//			myStack.push_back(keyword.getKeywordValue(token));
//		}
//
//	private:
//		KPascal::Keyword keyword;
//		KPascal::Lexer lexer;
//		std::vector<std::string> myStack;
//		char NatArray[15][15];
//		const std::string tableFileName = "table.txt";
//		std::ifstream fin;
//	};
//}
//
//#endif