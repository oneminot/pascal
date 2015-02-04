#ifndef ReservedKeyword_H_
#define ReservedKeyword_H_
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
namespace KPascal
{
	class ReservedKeyword
	{
	private:
		const std::string reservedKeywordFile = "reservedkeywords.txt";
		std::map<std::string, bool> reservedkeywords;
		std::ifstream fin;
	public:
		ReservedKeyword()
		{
			std::string myString;
			fin.open(reservedKeywordFile);
			if (fin.is_open())
			{
				while (!fin.eof())
				{
					std::getline(fin, myString);
					reservedkeywords.insert(std::make_pair(myString, true));
				}
				fin.close();
			}
		}
		bool IsKeywordReserved(std::string keyword)
		{
				if (reservedkeywords.find(keyword) == reservedkeywords.end())
				{
					return true;
				}
			return false;
		}
	};
}
#endif // !ReservedKeyword_H_
