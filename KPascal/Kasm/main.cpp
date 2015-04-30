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
		imul eax, 3
		mov ebx, 6
		imul ebx, 5
		mov ecx, 8
		imul ecx, 7
		add ebx, ecx
		add eax, ebx
		mov ebx, 10
		add ebx, 9
		sub eax, ebx
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
