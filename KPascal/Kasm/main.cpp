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
		mov [ebp + 4], eax
		mov eax, 3
		mov [ebp + 8], eax
		mov eax, 4
		mov [ebp + 12], eax
		mov eax, [ebp + 8]
		mov ebx, [ebp + 8]
		mov [ebp + 0], ebx
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
