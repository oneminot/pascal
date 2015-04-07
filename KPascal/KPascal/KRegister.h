#ifndef KRegister_H_
#define KRegister_H_ 
#include <string>
#include <map>
namespace KPascal
{
	struct KRegister
	{
		//General registers
		//EAX EBX ECX EDX

		//EAX, AX, AH, AL : Called the Accumulator register.
		//It is used for I / O port access, arithmetic, interrupt calls,
		//etc...

		//EBX, BX, BH, BL : Called the Base register
		//It is used as a base pointer for memory access
		//Gets some interrupt return values

		//ECX, CX, CH, CL : Called the Counter register
		//It is used as a loop counter and for shifts
		//Gets some interrupt values

		//EDX, DX, DH, DL : Called the Data register
		//It is used for I / O port access, arithmetic, some interrupt
		//calls.

		//from http://www.eecg.toronto.edu/~amza/www.mindsec.com/files/x86regs.html  

		//std::map<std::string, bool> kRegister;
		//kRegister.insert(std::make_pair("EAX", false);
		//kRegister.insert(std::make_pair("EBX", false);
		//kRegister.insert(std::make_pair("ECX", false);
		//kRegister.insert(std::make_pair("EDX", false);
		//kRegister.insert(std::make_pair("ESI", false);
		//kRegister.insert(std::make_pair("EDI", false);

		std::string RegisterName;
		bool IsUsed;
	};
}
#endif // !KRegister_H_
