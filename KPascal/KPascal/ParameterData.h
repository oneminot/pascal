#ifndef ParameterData_H_
#define ParameterData_H_
#include "Variable.h"
namespace KPascal
{
	struct ParameterData : KPascal::Variable
	{
		bool isPassedByReference;
	};
}
#endif // !ParameterData_H_
