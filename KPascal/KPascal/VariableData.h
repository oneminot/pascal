#ifndef VariableData_H_
#define VariableData_H_ 
#include <map>
#include "Variable.h"
#include "LocalVariable.h"
#include "ParameterData.h"
#include "KArrayDimension.h"
namespace KPascal
{
	struct VariableData : KPascal::Variable
	{
		std::map<std::string, KPascal::ParameterData> parameters;
		std::map<std::string, KPascal::LocalVariable> localvariables;
		std::vector<KArrayDimension> my_array_size;
	};
}
#endif // !VariableData_H_
