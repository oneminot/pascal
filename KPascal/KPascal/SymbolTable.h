#ifndef SymbolTable_H_
#define SymbolTable_H_
#include <map>
#include "VariableData.h"
namespace KPascal
{
	struct SymbolTable
	{
		std::map <std::string, VariableData> Table;
	};
}
#endif // !SymbolTable_H_
