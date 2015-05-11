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
		mov eax, 3
		add eax, 2
		mov ebx, 5
		add ebx, 4
		imul eax, ebx
		mov ebx, 7
		add ebx, 6
		mov ecx, 9
		add ecx, 8
		imul ebx, ecx
		add eax, ebx
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
