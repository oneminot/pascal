#ifndef VariableData_H_
#define VariableData_H_ 
#include <string>
#include <map>
#include "LocalVariable.h"
#include "ParameterData.h"
struct VariableData
{
	std::string type;
	int size;
	int offset;
	std::map<std::string, ParameterData> parameters;
	std::map<std::string, LocalVariable> localvariables;
};
#endif // !VariableData_H_
