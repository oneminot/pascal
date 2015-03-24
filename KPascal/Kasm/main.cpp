#include <iostream>
#include <fstream>
#include <string>

char DataSegment[65536];

int main()
{
	std::ifstream fin("..\\kAssembly.txt");
	std::string line;
	while (!fin.eof())
	{
		std::getline(fin, line);
	}
	_asm
	{

	}
	return 0;
}