#include "syscall.h"

int main()
{
	int id;
	int size;
	char fileName[32];
	int i; 
	char c; //Ky tu de in ra
	PrintString("\n\t\t\t-----CAT-----\n\n");
	PrintString(" File name: ");

	ReadString(fileName, 32);
	id = Open(fileName, 1); // Goi ham Open de mo file 
	
	if (id == -1) //loi
	{
		PrintString("can't open file\n\n");	
	}
	else
	{
		//Seek den cuoi file de lay size
		size = Seek(-1, id);
		// Seek den dau tap tin
		Seek(0,id);		
		PrintString("FILE:\n");
		for (i=0; i < size; i++) 
		{
			Read(&c, 1, id); // doc tung ky tu 
			PrintChar(c); 
		}
		Close(id); // dong file
	}
	return 0;
}
