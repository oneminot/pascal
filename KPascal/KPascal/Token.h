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
	};
}
#endif