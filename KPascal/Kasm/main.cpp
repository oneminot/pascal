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
		mov [ebp + 0], eax
		while0:
		mov eax, [ebp + 0]
		mov ebx, 5
		cmp eax, ebx
		jne  endwhile0
		mov eax, [ebp + 0]
		mov [ebp + 4], eax
		mov eax, 1
		add eax, [ebp + 0]
		mov [ebp + 0], eax
		jmp while0
		endwhile0:
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
