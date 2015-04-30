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
		mov eax, 5
		add eax, 4
		mov ebx, 2
		sub ebx, 3
		mov ecx, 6
		add ecx, 7
		mov edx, 8
		sub edx, 9
		imul ecx, edx
		imul ebx, ecx
		imul eax, ebx
		mov [ebp + 0], eax
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
