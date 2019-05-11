#include "syscall.h"
#include "copyright.h"

int main(int argc, char* argv[])
{
	int sourceID;
	char source[255];
	int fileSize,sourceSize, i;
	char c;
	
	PrintString("\nSource file:");
	ReadString(source, 255);

	sourceID = Open(source, 1);
	
	if (sourceID != -1) 
	{

		fileSize = Seek(-1, sourceID);
		PrintString("\n\nFile : ");
		PrintString(source);
		PrintString(" co kich thuoc: 	");
		PrintInt(fileSize);
		PrintString("	bytes\n\n");
		Close(sourceID); //dong file nguon
	}
	else
	{
		PrintString("\nLoi mo file!");
	}
	return 0;
}

