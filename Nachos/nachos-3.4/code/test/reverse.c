#include "syscall.h"
#include "copyright.h"

int main(int argc, char* argv[])
{
	int sourceID,destID;
	char source[255], dest[255];
	int fileSize,sourceSize, i;
	char c;
	
	PrintString("\nSource file:");
	ReadString(source, 255);
	PrintString("\nDestination file:");
	ReadString(dest, 255);

	sourceID = Open(source, 1);
	
	if (sourceID != -1) 
	{
		destID = CreateFile(dest);
		Close(destID);
		
		destID = Open(dest, 0); 
		if (destID != -1)
		{
			fileSize = Seek(-1, sourceID);
			Seek(0, destID); // Seek den dau file dich
			
			for (i=fileSize-1; i >=0; i--) 
			{
				Seek(i, sourceID);
				Read(&c, 1, sourceID); //Doc tung ki tu cua file nguon
				Write(&c, 1, destID); //Ghi vao file dich
			}
			
			PrintString("\nDa copy thanh cong!\n\n");
			Close(destID); // dong file dich
		}
		else
		{
			PrintString("\nKhong the tao file dich.\n\n");
		}
		
		Close(sourceID); //dong file nguon
	}
	else
	{
		PrintString("\nLoi mo file!");
	}
	
	return 0;
}
