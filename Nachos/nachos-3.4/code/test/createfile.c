#include "syscall.h"
int main()
{
	int stdin;
	char fileName[32];

	PrintString("\n\t\t\t----------CREATE FILE---------\n");
	PrintString("Nhap ten file: ");
	
	stdin = Open("stdin",2); // mo file stdin de nhap tu console

	if (stdin != -1)// mo file thanh cong
	{
		int lenght = Read(fileName, 32, stdin); // doc ten file vua nhap
		
		if(lenght >1)
		{
			if (CreateFile(fileName) == 0) // Goi ham CreateFile
			{
				PrintString("Tao file thanh cong!\n");
			}
		}
		else
		PrintString("File khong hop le!!!\n\n"); 
		Close(stdin); // close stdin
	}

	
	return 0;
}
