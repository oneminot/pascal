#include <iostream>
#include <fstream>
#include <string>

char DataSegment[65536];

int main()
{
	_asm
	{
		lea eax, DataSegment
			// put starting address of data segement into ebp
			mov ebp, eax
			//  x := -1;
			mov  eax, -1
			mov  DWORD PTR[ebp + 1], eax
			// y := 10;
			mov eax, 10
			mov DWORD PTR[ebp + 5], eax
			// z := x + y ;
			mov eax, DWORD PTR[ebp + 1]
			add eax, DWORD PTR[ebp + 5]
			mov DWORD PTR[ebp + 9], eax
			//if y=5 then
			cmp	dword ptr[ebp + 5], 5
			// y:=y+1;
			jne l1
			mov eax, dword ptr[ebp + 5]
			add eax, 1
			mov dword ptr[ebp + 5], eax
		l1 : mov eax, 0
			 //while y<15 do
		 l2 : cmp	dword ptr[ebp + 5], 15
			  // y:=y+1;
			  jge l3
			  mov eax, dword ptr[ebp + 5]
			  add eax, 1
			  mov dword ptr[ebp + 5], eax
			  jmp l2
		  l3 : mov eax, 0
	}
	return 0;
}