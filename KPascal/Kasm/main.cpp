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
		mov [ebp + 0], eax
		mov eax, [ebp + 0]
		mov ebx, 6
		cmp eax, ebx
		jge  endorelse0
		mov eax, 42
		mov [ebp + 0], eax
		jmp end0
		endorelse0:
		end0:
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
