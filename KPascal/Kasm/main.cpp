#include <fstream>
#include <iostream>
char DataSegment[65536];
int main()
{
	_asm
	{
		mov eax, 2
		mov [ebp + 40], eax
		mov eax, [ebp + 40]
		sub eax, 0
		imul eax, 4
		add eax, 0
		mov ebx, 10
		mov [ebp + eax], ebx
	}
	std::cin.get();
	return 0;
}
