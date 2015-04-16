#include "KRegister.h"
#ifndef KRegisterArray_H_
#define KRegisterArray_H_
namespace KPascal
{
	struct KRegisterArray
	{
		KRegister kRegisters[6];
		int currentRegisterIndex;
		KRegisterArray()
		{
			kRegisters[0].RegisterName = "eax";
			kRegisters[1].RegisterName = "ebx";
			kRegisters[2].RegisterName = "ecx";
			kRegisters[3].RegisterName = "edx";
			kRegisters[4].RegisterName = "esi";
			kRegisters[5].RegisterName = "edi";
			for (size_t i = 0; i < 6; i++)
			{
				kRegisters[i].IsUsed = false;
			}
			currentRegisterIndex = 0;
		}
	};
}
#endif // !KRegisterArray_H_
