#ifndef Variable_H_
#define Variable_H_
#include <string>
namespace KPascal
{
	struct Variable
	{
		std::string type;
		int size;
		int offset;
		bool isMethod;
	};
}
#endif // !Variable_H_
