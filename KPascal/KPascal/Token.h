#ifndef Token_H
#define Token_H
#include <string>

namespace KPascal
{
	struct Token
	{
		int type;
		std::string sType;
		std::string value;
		// add a boolean to tell whether something is a keyword 
	};
}
#endif