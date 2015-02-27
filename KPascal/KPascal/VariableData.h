#ifndef VariableData_H_
#define VariableData_H_ 
#include <string>
#include <map>
#include "Variable.h"
#include "LocalVariable.h"
#include "ParameterData.h"
namespace KPascal
{
	struct VariableData : KPascal::Variable
	{
		std::map<std::string, KPascal::ParameterData> parameters;
		std::map<std::string, KPascal::LocalVariable> localvariables;
	};
}
#endif // !VariableData_H_
