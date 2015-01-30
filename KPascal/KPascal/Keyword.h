#ifndef Keyword_H_
#define Keyword_H_
#include "Token.h"
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
namespace KPascal
{
	class Keyword
	{
	private:
		const std::string keywordFile = "keyword.txt";
		std::map<std::string, std::string> keywords;
		std::ifstream fin;
	public:
		Keyword()
		{
			std::string myString;
			std::ifstream fin;
			fin.open(keywordFile);
			std::vector<std::string> stuff;
			if (fin.is_open())
			{
				while (!fin.eof())
				{
					std::getline(fin, myString);
					stuff = split(myString, ' ');
					keywords.insert(std::make_pair(stuff[1], stuff[0]));
				}
				fin.close();
			}
		}
		
		std::string getKeywordValue(std::string key)
		{
			return keywords[key];
		}

		std::string getKeywordValue(Token token)
		{
			if (token.sType == "special")
			{
				return keywords[token.value];
			}
			return keywords[token.sType];
		}

		std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
		{
			std::stringstream ss(s);
			std::string item;
			while (getline(ss, item, delim))
			{
				elems.push_back(item);
			}
			return elems;
		}

		std::vector<std::string> split(const std::string &s, char delim)
		{
			std::vector<std::string> elems;
			split(s, delim, elems);
			return elems;
		}
	};
}

#endif