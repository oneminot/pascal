#include <fstream>
char DataSegment[65536];
int main()
{
	_asm
	{
		lea eax, DataSegment
			mov ebp, eax
			mov EAX, 2
	}
	return 0;
}
