#ifndef ParameterData_H_
#define ParameterData_H_
#include <string>
struct ParameterData
{
	std::string type;
	int size;
	int offset;
	bool isPassedByReference;
};
#endif // !ParameterData_H_
