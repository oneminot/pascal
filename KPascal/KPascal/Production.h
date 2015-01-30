#ifndef Production_H_
#define Production_H_
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
namespace KPascal
{
	class Production
	{
	private:
		std::string value;
		const std::string productionFile = "production.txt";
		std::map<std::string, std::string> productions;
		std::ifstream fin;
		std::vector<std::string> MyStack;
	public:
		Production()
		{
			std::string myString;
			std::ifstream fin;
			fin.open(productionFile);
			std::vector<std::string> stuff;
			//put nothing ($) on the stack 
			MyStack.push_back("15");
			if (fin.is_open())
			{
				while (!fin.eof())
				{
					std::getline(fin, myString);
					stuff = split(myString, ' ');
					productions.insert(std::make_pair(stuff[1], stuff[0]));
				}
				fin.close();
			}
		}
		std::string getProductionValue(std::string key)
		{
			return productions[key];
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
#endif // !Production_H_
