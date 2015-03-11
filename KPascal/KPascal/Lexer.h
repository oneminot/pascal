#ifndef Lexer_H
#define Lexer_H
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "Token.h"
#include "ReservedKeyword.h"
namespace KPascal
{
	class Lexer
	{
	private:
		const static int numberOfStates = 13;
		const static int numberOfAsciiValuesOfInput = 127;
		const std::string dfaFileName = "dfa.txt";
		const std::string programFileName = "ksample.txt";
		int kLanguage[numberOfAsciiValuesOfInput][numberOfStates];
		int kColumn;
		int currentState, previousState;
		std::ifstream fin;
		KPascal::ReservedKeyword reservedkeyword;
		//std::stringstream kLine;
	public:
		Lexer()
		{
			fin.open(dfaFileName);
			if (fin.is_open())
			{
				for (int i = 0; i < numberOfAsciiValuesOfInput; i++)
				{
					for (int j = 0; j < numberOfStates; j++)
					{
						fin >> kLanguage[i][j];
					}
				}
			}
			fin.close();
			fin.open(programFileName);
		};
		~Lexer()
		{

		};
		bool getToken(Token & kToken)
		{
			currentState = 0;
			previousState = 0;
			kToken.value.clear();
			while (true)
			{
				char myCharacter = fin.peek();
				previousState = currentState;
				currentState = kLanguage[myCharacter][currentState];
				kToken.value.push_back(myCharacter);
				if (currentState == 55 || myCharacter == -1)
				{
					if (previousState == 0)
					{
						if (myCharacter == -1)
						{
							return false;
						}
						currentState = 0;
						kToken.value.clear();
					}
					else
					{
						kToken.value.pop_back();
						kToken.type = previousState;						
						switch (kToken.type)
						{
						case 1:
							kToken.sType = "word";
							break;
						case 2:
							kToken.sType = "integer";
							break;
						case 3:
							kToken.sType = "special";
							break;
						case 5:
							kToken.sType = "real";
							break;
						case 10:
							kToken.sType = "real";
							break;
						case 11: 
							kToken.sType = "special";
							break;
						default:
							;//do nothing
						}
						std::transform(kToken.value.begin(), kToken.value.end(), kToken.value.begin(), ::tolower);
						if (reservedkeyword.IsKeywordReserved(kToken.value))
						{
							kToken.isKeyword = true;
						}
						else
						{
							kToken.isKeyword = false;
						}
						std::cout << kToken.value << std::endl;
						return true;
					}
				}
				//else if (currentState == 11)
				//{
				//	kToken.type = 3;
				//	if (reservedkeyword.IsKeywordReserved(kToken.value))
				//	{
				//		kToken.isKeyword = true;
				//	}
				//	else
				//	{
				//		kToken.isKeyword = false;
				//	}
				//	kToken.sType = "special";
				//	return true;
				//}
				else if (currentState == 99)
				{
					std::cout << "oops";
					std::cin.get();
					exit(1);
				}
				fin.get();
			}
		}
	};
}
#endif