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
		mov ebx, 5
		cmp eax, ebx
		jne  endorelse1
		mov ecx, 22
		mov [ebp + 0], ecx
		jmp end1
		endorelse1:
		mov ecx, 33
		mov [ebp + 0], ecx
		mov ecx, 1
		neg ecx
		add ecx, [ebp + 0]
		mov [ebp + 0], ecx
		end1:
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
