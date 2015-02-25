#ifndef ParameterData_H_
#define ParameterData_H_
#include <string>
#include "Variable.h"
struct ParameterData : Variable
{
	bool isPassedByReference;
};
#endif // !ParameterData_H_
