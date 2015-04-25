#include <fstream>
#include <iostream>
char DataSegment[65536];
int main()
{
	_asm
	{
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
		push esp
		push ebp
		lea eax, DataSegment
		mov ebp, eax
		mov eax, 2
		imul eax, 2
		mov ebx, 3
		imul ebx, 3
		mov ecx, 4
		imul ecx, 4
		mov edx, 5
		imul edx, 5
		mov esi, 6
		imul esi, 6
		mov [ebp + 0], esi
		pop ebp
		pop esp
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
	}
	std::cin.get();
	return 0;
}
