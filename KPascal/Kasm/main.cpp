#include <fstream>
#include <iostream>
char DataSegment[65536];
int main()
{
	_asm
	{
		lea eax, DataSegment
		mov ebp, eax
		mov eax, 3
		sub eax, 2
		imul eax, 400
		mov ebx, 4
		sub ebx, 3
		imul ebx, 40
		mov ecx, 4
		sub ecx, 3
		imul ecx, 4
		mov edx, 25
		mov [ebp + 0], edx
	}
	std::cin.get();
	return 0;
}
