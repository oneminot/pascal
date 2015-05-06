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
		jne  endorelse0
		mov eax, 22
		mov [ebp + 0], eax
		mov eax, 1
		add eax, [ebp + 0]
		mov [ebp + 0], eax
		mov eax, [ebp + 0]
		mov ebx, 6
		cmp eax, ebx
		jne  endorelse1
		mov eax, 42
		mov [ebp + 0], eax
		jmp end1
		endorelse1:
		mov eax, 92
		mov [ebp + 0], eax
		end1:
		jmp end0
		endorelse0:
		mov eax, 33
		mov [ebp + 0], eax
		mov eax, 1
		neg eax
		add eax, [ebp + 0]
		mov [ebp + 0], eax
		end0:
		mov eax, [ebp + 0]
		add eax, [ebp + 0]
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
