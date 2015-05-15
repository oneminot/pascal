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
		jge  endwhile0
		mov eax, [ebp + 0]
		mov [ebp + 4], eax
		mov eax, 1
		add eax, [ebp + 0]
		mov [ebp + 0], eax
		mov eax, [ebp + 4]
		mov ebx, 5
		cmp eax, ebx
		jne  endorelse0
		while1:
		mov eax, [ebp + 4]
		mov ebx, 20
		cmp eax, ebx
		jge  endwhile1
		mov eax, 5
		add eax, [ebp + 4]
		mov [ebp + 4], eax
		jmp while1
		endwhile1:
		jmp end0
		endorelse0:
		end0:
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
