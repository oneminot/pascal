#include <iostream>


using namespace std;

char DataSegment[65536];

// var done : boolean;  (done would take the first by in data location 0)
// var x,y,z : integer; (x uses 4 bytes starting at location 1)
//                      (y uses 4 bytes starting at location 5)
//						(z uses 4 bytes starting at location 9)
// begin
//		x := -1;
//      y :=  10;
//      z := x + y ;
// end.

int x, y, z, w;



int main()
{
	x = 2;
	y = 3;
	z = 4;

	w = x + y;

	w = x + y*z;

	w = z*x + y*z;

	if (x == 5)
		x++;

	_asm{
		lea eax, DataSegment
			mov ebp, eax    // put starting address of data segement into ebp

			mov  eax, -1				   //  x := -1;
			mov  DWORD PTR[ebp + 1], eax

			mov eax, 10				   // y := 10;
			mov DWORD PTR[ebp + 5], eax

			mov eax, DWORD PTR[ebp + 1]  // z := x + y ;
			add eax, DWORD PTR[ebp + 5]
			mov DWORD PTR[ebp + 9], eax

			cmp	dword ptr[ebp + 5], 5       //if y=5 then
			jne l1						  // y:=y+1;
			//		
			mov eax, dword ptr[ebp + 5]     //
			add eax, 1					  //
			mov dword ptr[ebp + 5], eax     //
			//	
		l1 : mov eax, 0				      //


		 l2 : cmp	dword ptr[ebp + 5], 15      //while y<15 do
			  jge l3						  // y:=y+1;
			  //		
			  mov eax, dword ptr[ebp + 5]     //
			  add eax, 1					  //
			  mov dword ptr[ebp + 5], eax     //
			  jmp l2					      //	
		  l3 : mov eax, 0					  //


	}
	return 0;
}