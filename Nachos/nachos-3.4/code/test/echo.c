#include "syscall.h"
#include "copyright.h"

int main(int argc, char* argv[])
{
	int out,in;
	char buffer[255];
	int lenght; 
	PrintString("\n\t\t\t-----ECHO-----\n\n");
	PrintString("INPUT: ");
 
	in = Open("stdin", 2);
	if (in != -1)
	{
		lenght = Read(buffer, 255, in);
		
		if (lenght != -1 && lenght != -2) 
		{
			out = Open("stdout", 3); 
			if (out != -1)
			{
				PrintString("OUTPUT: ");
				Write(buffer,lenght,out); //ghi vao stdout
				Close(out); // close stdout
			}
		}
		Close(in); // close stdin
	}
	PrintString("\n\n");
	return 0;
}
