#ifndef SymbolTable_H_
#define SymbolTable_H_
#include <map>
#include <string>
#include "ParameterData.h"
#include "LocalVariable.h"
#include "VariableData.h"
struct SymbolTable
{
	std::map <std::string, VariableData> Table;
};
#endif // !SymbolTable_H_
