#include <fstream>
#include <string>
#include <map>
int main()
{
	const std::string input = "E.in";
	const std::string output = "E.out";
	std::string line;
	std::ifstream fin(input);
	std::map<char, int> letters;
	while (!fin.eof())
	{
		std::getline(fin, line);
		if (line == "*")
		{
			break;
		}
	}
}