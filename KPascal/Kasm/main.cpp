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
		mov eax, 0
		sub eax, 0
		imul eax, 4
		add eax, 0
		mov ebx, 10
		mov [ebp + eax], ebx
		mov ebx, 1
		sub ebx, 0
		imul ebx, 4
		add ebx, 0
