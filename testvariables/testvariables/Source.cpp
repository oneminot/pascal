#include <fstream>
#include <string>
#include <map>
int main()
{
	const std::string input = "E.in";
	std::ifstream fin(input);
	std::map<char, int> letters;
	for (char i = 'a'; i < 'z'; i++)
	{
		letters.insert(std::pair<char, int>(i, 0));
	}
	for (size_t i = 0; i < 35; i++)
	{

	}
}