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
		mov eax, 4
		neg eax
		add eax, 3
		add eax, 2
		mov [ebp + 0], eax
		mov eax, 45
		add eax, 44
		neg eax
		add eax, 43
		imul eax, 42
		mov [ebp + 4], eax
		mov eax, 3
		sub eax, 2
		imul eax, 400
		mov ebx, 4
		sub ebx, 3
		imul ebx, 40
		add eax, ebx
		mov ebx, 4
		sub ebx, 3
		imul ebx, 4
		add eax, ebx
		add eax, 8
		mov ebx, 25
		mov [ebp + eax], ebx
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
