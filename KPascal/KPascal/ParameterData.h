#ifndef ParameterData_H_
#define ParameterData_H_
#include <string>
#include "Variable.h"
namespace KPascal
{
	struct ParameterData : KPascal::Variable
	{
		bool isPassedByReference;
	};
}
#endif // !ParameterData_H_
