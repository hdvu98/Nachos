// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MaxFileLength 32 //Do dai ten file toi da la 32

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

//Program counter
void IncreasePC();
char* User2System(int virtAddr,int limit)
{
	int i;
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1];
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0,limit + 1);
	for (i = 0; i < limit; i++)
	{
	  machine->ReadMem(virtAddr + i, 1, &oneChar);
	  kernelBuf[i] = (char)oneChar;
	  if (oneChar == 0) 
		break;
	}
	return kernelBuf;
}

int System2User(int virtAddr,int len, char* buffer)
{
	if (len < 0) return -1;
 	if (len == 0) return len;
	int i = 0;
	int oneChar = 0;
	do{
	  oneChar = (int)buffer[i];
	  machine->WriteMem(virtAddr + i, 1, oneChar);
	  i++;
	}while(i < len && oneChar !=0);
	return i;
}

//Ham su li runtime exception va system call 
void ExceptionHandler(ExceptionType which)
{

    int type = machine->ReadRegister(2);//Bien toan cuc cho lop SynchConsole

    switch(which)
   {
	case NoException:
		return;

	case PageFaultException:
		DEBUG('a',"\n No valid translation found");
		printf("\n No valid translation found");
		interrupt->Halt();
		break;

	case ReadOnlyException:
		DEBUG('a',"\n Write attempted page marked read-ony");
		printf("\n Write attempted page marked read-only");
		interrupt->Halt();
		break;

	case BusErrorException:
		DEBUG('a',"\n Translation resulted invalid physical address");
		printf("\n Translation resulted invalid physical address");
		interrupt->Halt();
		break;

	case AddressErrorException:
		DEBUG('a',"\n Unaligned reference or one that beyond the end of the address space");
		printf("\n Unaligned reference or one that beyond the end of the address space");
		interrupt->Halt();
		break;

	case OverflowException:
		DEBUG('a',"\n Integer overflow in add or sub.");
		printf("\n Integer overflow in add or sub.");
		interrupt->Halt();
		break;

	case IllegalInstrException:
		DEBUG('a',"\n Unimplemented or reserved instr.");
		printf("\n Unimplemented or reserved instr.");
		interrupt->Halt();
		break;

	case NumExceptionTypes:
		DEBUG('a',"\n Number exception type");
		printf("\n Number exception type:");
		interrupt->Halt();
		break;

	case SyscallException:
	     switch(type)
            {
		case SC_Halt:
		{ 
		 DEBUG('a', "\nShutdown, initiated by user program.\n");
		 printf("\nShutdown, initiated by user program.\n");
   	         interrupt->Halt();
		 return;
		}
		case SC_ReadInt:
		{
                    char* buffer;
                    int MAX_BUFFER = 255;
                    buffer = new char[MAX_BUFFER + 1];
                    int numbytes = gSynchConsole->Read(buffer, MAX_BUFFER);// doc ky tu nhap vao, tra ve do dai chuoi luu vao numbytes
                    int num = 0; // so luu ket qua tra ve cuoi cung		
                    // bien isNegative danh dau so nhap vao la - hay +                       
                    bool isNegative = false; // Gia thiet la so duong.
                    int first = 0;
                    int last = 0;
                    if(buffer[0] == '-')
                    {
                        isNegative = true;
                        first = 1;
                        last = 1;                        			   		
                    }
		    if(buffer[0]=='+')
			{
			first=1;
			last=1;
			}
                    for(int i = first; i < numbytes; i++)					
                    {
                        if(buffer[i] == '.') //1.0 la so nguyen
                        {
                            int j = i + 1;
                            for(j; j < numbytes; j++)
                            {
                                if(buffer[j] != '0')// vi du 1.05 khong phai so nguyen
                                {
                                    printf("\n\n The integer number is not valid");
                                    DEBUG('a', "\n The integer number is not valid");
                                    machine->WriteRegister(2, 0);
                                    IncreasePC();
                                    delete buffer;
                                    return;
                                }
                            }
                            // la so nguyen
                            last = i - 1;				
                            break;                           
                        }
                        else if(buffer[i] < '0' && buffer[i] > '9')// khong phai ki tu tu 0->9
                        {
                            printf("\n\n The integer number is not valid");
                            DEBUG('a', "\n The integer number is not valid");
                            machine->WriteRegister(2, 0);
                            IncreasePC();
                            delete buffer;
                            return;
                        }
                        last = i;    
                    }			
                    
                    // chuyen "1234"->1234
                    for(int i = first; i<= last; i++)
                    {
                        num = num * 10 + (int)(buffer[i] - '0'); 
                    }
                    
                    // la so (-)
                    if(isNegative)
                    {
                        num = -1*num;
                    }
                    machine->WriteRegister(2, num);
                    IncreasePC();
                    delete buffer;
                    return;		
		}
		case SC_PrintInt:
		{
            	int num = machine->ReadRegister(4);
		if(num == 0)
                {
                        gSynchConsole->Write("0", 1); // In ra man hinh so 0
                        IncreasePC();
                        return;    
                }  
           	bool isNegative = false; // +
            	int length = 0; // Bien de luu so chu so cua number
            	int first = 0; 
			
            	if(num < 0)
                    {
                        isNegative = true;
                        num = num * -1; // (-)->(+)
                        first = 1; //bat dau tu vi tri dau tien sau dau (-)
                    } 	
                    
            int temp_num = num; 
            while(temp_num)//dem so chu so
                    {
                        length++;
                        temp_num =temp_num/10;
                    }

            char* buffer=new char[256];
            for(int i = first + length - 1; i >= first; i--)
                    {
                        buffer[i] = (char)((num % 10) + 48);
                        num=num/ 10;
                    }
            if(isNegative)//(-)
                    {
                        buffer[0] = '-';
			buffer[length + 1] = 0;
                        gSynchConsole->Write(buffer, length + 1);
                        delete buffer;
                        IncreasePC();
                        return;
		    }
		    buffer[length] = 0;	
            gSynchConsole->Write(buffer, length);
            delete buffer;
            IncreasePC();
            return;     	
		}
	case SC_ReadChar:
		//input: không có
		//output: char
		{
			char* buffer = new char[255];
			int length = gSynchConsole->Read(buffer, 255);

			if(length > 1) //char: 1 ky tu
			{
				printf("Kieu char chi duoc nhap 1 ky tu!");
				DEBUG('a', "\nERROR: Kieu char chi duoc nhap 1 ky tu!");
				machine->WriteRegister(2, 0);
			}
			else if(length == 0) //Ky tu rong
			{
				printf("Nhap khong hop le!");
				DEBUG('a', "\nERROR: Nhap khong hop le!");
				machine->WriteRegister(2, 0);
			}
			else
			{
				char c = buffer[0];
				machine->WriteRegister(2, c);
			}

			delete buffer;
			IncreasePC();
			break;
		}
		case SC_PrintChar:
		{
			char c = (char)machine->ReadRegister(4); //thanh ghi r4 luu tham so dau vao
			gSynchConsole->Write(&c, 1); // In ky tu
			//IncreasePC();
			break;
		}
		case SC_ReadString:
		{
			int p, strLength;
			char *buffer=new char[255];
			p = machine->ReadRegister(4); // lấy địa chỉ từ thanh ghi r4
			strLength = machine->ReadRegister(5); // độ dài chuỗi từ thanh ghi r5
			buffer = User2System(p, strLength); 
			gSynchConsole->Read(buffer, strLength); // đọc chuỗi 
			System2User(p, strLength, buffer); 
			delete buffer; 
			IncreasePC(); 
			break;
		}
		case SC_PrintString:
		{
			int length = 0;//lưu độ dài chuỗi
			int p;//lấy địa chỉ tham số đầu vào
			char* buffer;
			p = machine->ReadRegister(4); // lấy địa chỉ tham số đầu vào từ r4
			buffer = User2System(p, 255); // copy chuỗi từ User sang System Space
			while (buffer[length] != 0) //tìm lenght của chuỗi
				length++; 
			gSynchConsole->Write(buffer, length + 1);
			delete buffer; 
			//IncreasePC();
			//return;
			break;
		}
	case SC_CreateFile:
		{
			// Chuc nang tao file voi ten file truyen vao
			// Output: return -1 neu loi, return 0 neu thanh cong

			int virtualAddr;
			char* fileName;
			
			DEBUG('a', "\n SC_CreateFile call ...");
			DEBUG('a', "\n Reading virtual address of filename");

			virtualAddr = machine->ReadRegister(4); //lấy địa chỉ file từ R4
			
			DEBUG('a', "\n Reading filename.");
			
			//Copy khong gian bo nho User -> System
			
			fileName = User2System(virtualAddr, MaxFileLength + 1);
			
			if (strlen(fileName) == 0)
			{
				printf("\n File name is not valid");
				DEBUG('a', "\n File name is not valid");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				//IncreasePC();
				//return;
				break;
			}
			
			if (fileName == NULL)  //Khong doc duoc file
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				delete fileName;
				//IncreasePC();
				//return;
				break;
			}
			
			DEBUG('a', "\n Finish reading filename.");
			
			if (!fileSystem->Create(fileName, 0)) //neu tao file that bai
			{
				printf("\n Error create file '%s'", fileName);
				machine->WriteRegister(2, -1);
				delete fileName;
				//IncreasePC();
				//return;
				break;
			}
			
			//Tao file thanh cong
			machine->WriteRegister(2, 0);
			delete fileName;
			//IncreasePC(); //Day thanh ghi lui ve sau de tiep tuc ghi
			//return;
			break;
		}
		case SC_Open:
		{
			// Input: dia chi cua ten tep tin, type
			// Output: return OpenFileID neu thanh cong, -1 neu loi
			// Chuc nang: tim OpenFileID
	 
			int virtualAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
			int type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
			char* fileName;
			fileName = User2System(virtualAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space voi bo dem name dai MaxFileLength
			
			int freeSlot = fileSystem->FindFreeSlot();
			if (freeSlot != -1) //con slot
			{
			
				if (type == 0 || type == 1) //type 0||1
				{
					
					if ((fileSystem->openf[freeSlot] = fileSystem->Open(fileName, type)) != NULL) //Mo file thanh cong
					{
						machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
					}
				}
				
				else if (type == 2) //type 2: stdout
				{
					machine->WriteRegister(2, 0); 
				}
				
				else // type 3: stdout
				{
					machine->WriteRegister(2, 1); 
				}
				delete[] fileName;
				break;
			}
			machine->WriteRegister(2, -1); //Khong mo duoc file
			
			delete[] fileName;
			break;
		}
		case SC_Close:
		{
			//chuc nang dong file
			//Input OpenFileID
			// Output return  0-> thanh cong, return -1 -> that bai
			int id = machine->ReadRegister(4); // ID tu thanh ghi R4
			
			if (id >= 0  && id <= 14) //xu ly khi id tu 0->14
			{
				if (fileSystem->openf[id]) //mo file thanh cong
				{
					delete fileSystem->openf[id]; //Xoa vung nho luu tru file
					fileSystem->openf[id] = NULL; 
					machine->WriteRegister(2, 0);
					break;
				}
			}
			
			machine->WriteRegister(2, -1);
			break;
		}
		case SC_Read:
			{
			// Doc file 
			// Input: buffer, so ky tu, id cua file
			// Output: tra ve -1: Loi, So byte thuc su: Thanh cong, -2: Thanh cong
			
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua buffer tu R4
			int charcount = machine->ReadRegister(5); // Lay so luong ky tu tu thanh ghi  R5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi R6
			
			int oldPos;
			int newPos;
			char *buffer;
			
			if (id < 0 || id > 14) // id nam ngoai bang mo ta file
			{
				printf("\nID nam ngoai bang mo ta file\n.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			
			// Kiem tra su ton tai cua file
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nfile khong ton tai\n.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			if (fileSystem->openf[id]->type == 3) //stdout
			{
				printf("\nKhong the doc file stdout.");
				machine->WriteRegister(2, -1); //return -1
				IncreasePC();
				return;
			}
			
			oldPos = fileSystem->openf[id]->GetCurrentPos(); // lay vi tri old pos
			buffer = User2System(virtAddr, charcount); // Copy chuoi tu vung nho User Space sang System Space
			
			// file stdin 
			if (fileSystem->openf[id]->type == 2)
			{
				int size = gSynchConsole->Read(buffer, charcount); 
				System2User(virtAddr, size, buffer); // Copy chuoi tu vung nho System Space sang User Space
				machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
				delete buffer;
				IncreasePC();
				return;
			}

			if ((fileSystem->openf[id]->Read(buffer, charcount)) > 0)
			{
				newPos = fileSystem->openf[id]->GetCurrentPos();
				// Copy chuoi tu  System Space sang User Space voi bo dem buffer co do dai la so byte
				int bytes=newPos-oldPos;
				System2User(virtAddr, bytes, buffer); 
				machine->WriteRegister(2, bytes);
			}
			else //file rong
			{
				//printf("\nfile rong.");
				machine->WriteRegister(2, -2);
			}
			
			delete buffer;
			IncreasePC();
			return;
			}
		case SC_Write:
		{
			// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
			// Output: loi -1, thanh cong: -2/ so byte da ghi
			int virtAddr = machine->ReadRegister(4); 
			int charcount = machine->ReadRegister(5);
			int id = machine->ReadRegister(6); 
			int oldPos;
			int newPos;
			char *buffer;

			if (id < 0 || id > 14)
			{
				printf("\nID khong thuoc bang mo ta file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (fileSystem->openf[id] == NULL)
			{
				printf("\nFile khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)// read only, stdin
			{
				printf("\nKhong the ghi file.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			oldPos = fileSystem->openf[id]->GetCurrentPos(); // lay old pos
			buffer = User2System(virtAddr, charcount);  // Copy chuoi tu vung nho User Space sang System Space
			//ghi file read & write (type quy uoc la 0) thi tra ve so byte thuc su
			if (fileSystem->openf[id]->type == 0)
			{
				if ((fileSystem->openf[id]->Write(buffer, charcount)) > 0)
				{
					newPos = fileSystem->openf[id]->GetCurrentPos();
					int bytes=newPos-oldPos;
					machine->WriteRegister(2, bytes);
					delete buffer;
					IncreasePC();
					return;
				}
			}
			if (fileSystem->openf[id]->type == 3) // Xet truong hop con lai ghi file stdout 
			{
				int i = 0;
				while ( buffer[i] != '\n' && buffer[i] != 0) 
				{
					gSynchConsole->Write(buffer + i, 1); // ham Write cua lop SynchConsole 
					i++;
				}
				buffer[i] = '\n';
				gSynchConsole->Write(buffer + i, 1); // Write ky tu '\n'
				machine->WriteRegister(2, i - 1); // tra ve so bytes
				delete buffer;
				IncreasePC();
				return;
			}
		}
		case SC_Seek:
		{
			// Input: Vi tri, id cua file
			// Output: -1: Loi, Vi tri thuc su: Thanh cong
			
			int position = machine->ReadRegister(4); // Lay vi tri can chuyen con tro den trong file tu R4
			int id = machine->ReadRegister(5); // Lay id cua file tu R5
			 
			if (id < 0 || id > 14)
			{
				printf("\nID nam ngoai bang mo ta.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			
			if (fileSystem->openf[id] == NULL)// file khong ton tai
			{
				printf("\nFile nay khong ton tai.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}

			if (id == 0 || id == 1)// goi seek tren console
			{
				printf("\nKhong the seek tren console.");
				machine->WriteRegister(2, -1);
				IncreasePC();
				return;
			}
			
			if(position==-1)
			{
				position=fileSystem->openf[id]->Length();
			}
			
			if (position > fileSystem->openf[id]->Length() || position < 0) //position khong hop le
			{
				printf("\nposition khong hop le.");
				machine->WriteRegister(2, -1);
			}
			else
			{
				fileSystem->openf[id]->Seek(position);
				machine->WriteRegister(2, position);
			}
			IncreasePC();
			return;
		}
	default:
		IncreasePC();
		break;
   }
   	default:
		IncreasePC();
		break;
   }
}
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}

