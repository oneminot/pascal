#include "KRegister.h"
#ifndef KRegisterArray_H_
#define KRegisterArray_H_
struct KRegisterArray
{
	KRegister kRegisters[];
	bool PopulateRegisters()
	{
		kRegisters[0].RegisterName = "EAX";
		kRegisters[1].RegisterName = "EBX";
		kRegisters[2].RegisterName = "ECX";
		kRegisters[3].RegisterName = "EDX";
		kRegisters[4].RegisterName = "ESI";
		kRegisters[5].RegisterName = "EDI";
		for (size_t i = 0; i < 5; i++)
		{
			kRegisters[i].IsUsed = false;
		}
	}
};
#endif // !KRegisterArray_H_
