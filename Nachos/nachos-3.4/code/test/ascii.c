#include "syscall.h"
#include "copyright.h"
int main()
{
	int i=32;
	for(i;i<=127;i++)
	{
		PrintInt(i);
		PrintChar('\t');
		PrintChar((char)i);
		PrintChar('\n');
	}
	return 0;
}

