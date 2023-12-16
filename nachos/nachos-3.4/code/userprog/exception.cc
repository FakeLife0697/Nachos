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

#define MaxFileLength 32

void increase_pc() {

    int current = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, current);
    current = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, current);
    machine->WriteRegister(NextPCReg, current + 4);
}

char* User2System(int virtAddr,int limit) 
{ 
	 int i;// index 
	 int oneChar; 
	 char* kernelBuf = NULL; 
	 
	 kernelBuf = new char[limit +1];//need for terminal string 
	 if (kernelBuf == NULL) 
	 	return kernelBuf; 
	 memset(kernelBuf,0,limit+1); 
	 
	 //printf("\n Filename u2s:"); 
	 for (i = 0 ; i < limit ;i++) 
	 { 
		 machine->ReadMem(virtAddr+i,1,&oneChar); 
		 kernelBuf[i] = (char)oneChar; 
		 //printf("%c",kernelBuf[i]); 
	 if (oneChar == 0) 
	 	break; 
	 } 
	 return kernelBuf; 
} 

int System2User(int virtAddr,int len,char* buffer) 
{ 
	 if (len < 0) return -1; 
	 if (len == 0)return len; 
	 int i = 0; 
	 int oneChar = 0 ; 
	 do{ 
		 oneChar= (int) buffer[i]; 
		 machine->WriteMem(virtAddr+i,1,oneChar); 
	 i ++; 
	 }
	while(i < len && oneChar != 0); 
	 return i; 
}

void SC_Create_execution() {

	int virtAddr;
	char* filename;
	
	DEBUG('a', "\n SC_Create call ...");
	DEBUG('a', "\n Reading virtual address of filename");
	
	virtAddr = machine->ReadRegister(4);
	DEBUG('a', "\n Reading filename");
	filename = User2System(virtAddr, MaxFileLength + 1);
	if (filename == NULL) {
	
		printf("\n Not enough memory in system");
		DEBUG('a', "\n Not enough memory in system");
		machine->WriteRegister(2, -1);
		delete filename;
		return;
	}
	
	DEBUG('a', "\n Finish reading filename");
	
	if (! fileSystem->Create(filename, 0)) {
	
		printf("\n Error create file '%s'", filename);
		machine->WriteRegister(2, -1);
		delete filename;
		return;
	}
	
	machine->WriteRegister(2, 0);
	
	delete filename;
	return;
}

void SC_Open_execution() {
	
	
}
void SC_ReadInt_execution() {
	char* buffer = new char[255];
	int len = gSynchConsole->Read(buffer, 256);

	int i = 0;
	if (buffer[0] == '-')
		i++;
	for (; i < len; i++) {

		if (buffer[i]  < '0' || buffer[i] > '9') {
			machine->WriteRegister(2, 0);
			return;
		}
	}
	int res = atoi(buffer);
	machine->WriteRegister(2, res);
	return;
}

void SC_PrintInt_execution() {

	int number = machine->ReadRegister(4);

	if (number == 0) {
		gSynchConsole->Write("0", 1);
		return;
	}
	bool is_negative = 0;
	int temp = abs(number);
	int len = 0;


	if (number < 0)
		is_negative = 1;
	
	while (temp != 0)  {
		len++;
		temp = temp / 10;
	}

	if (is_negative)
		len++;

	char* result = new char[len + 1];
	result[len] = '\0';

	int i = len - 1;
	temp = abs(number);
	while (temp != 0) {
		result[i] = (char)((temp % 10) + '0');
		temp = temp /10;
		i--;
	}
	if (is_negative)
		result[0] = '-';

    gSynchConsole->Write(result, len + 1);
}

void SC_ReadChar_execution() {

	char* buffer = new char[255];
	int len = gSynchConsole->Read(buffer, 256);

	machine->WriteRegister(2, buffer[0]);
	delete buffer;
	return;
}

void SC_PrintChar_execution() {
	char result = (char)(machine->ReadRegister(4));
	gSynchConsole->Write(&result, 1);
}

void SC_ReadString_execution() {

	int virtAddr = machine->ReadRegister(4);
	int len = machine->ReadRegister(5);

	char* buffer = new char [len + 1]; // 1 is terminated character
	gSynchConsole->Read(buffer, len); // read "len" character into buffer
	buffer[len] = '\0'; 			 // set terminated character

	System2User(virtAddr, len + 1, buffer); // 1 is terminated character
	delete[] buffer;
	return;
}

void SC_PrintString_execution() {

	int virtAddr = machine->ReadRegister(4);
	char* buffer = User2System(virtAddr, 256); 
	
	int len = 0;
	while (buffer[len] != '\0') {
		len++;
	}
	gSynchConsole->Write(buffer, len);
	delete[] buffer;
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch(which) {

    	case NoException:
    		break;

    	// SYSCALL EXCEPTIONS

    	case SyscallException: {

			switch(type) {

    			case SC_Halt: {
					DEBUG('a', "\nShutdown, initiated by user program");
					printf("\nShutdown, initiated by user program");
    				interrupt->Halt();
    				break;
				}
					
				
    			case SC_Create: {
					SC_Create_execution();
					break;
				}

			// #5 syscall
				// case SC_Open: {
				// 	SC_Open_execution();
				// 	break;
					
				// }


				case SC_ReadInt: {
					SC_ReadInt_execution();
					break;
				}

				case SC_PrintInt: {
					SC_PrintInt_execution();
					break;
				}

				case SC_ReadChar: {
					SC_ReadChar_execution();
					break;
				}

				case SC_PrintChar: {
					SC_PrintChar_execution();
					break;
				}

				case SC_ReadString: {
					SC_ReadString_execution();
					break;
				}

				case SC_PrintString: {
					SC_PrintString_execution();
					break;
				}
				
				default: {
					printf("\n Unexpected user mode exception");
					
					break;
				}						
			}

			increase_pc();
			break;
		}
    						
    	// OTHER EXCEPTIONS (7 exceptions)

    	case PageFaultException: {
			DEBUG('a', "\nNo valid translation found");
    		printf("\nNo valid translation found");
    		interrupt->Halt();
    		break;
		}
    		

    	case ReadOnlyException: {
			DEBUG('a', "\nWrite attempted to page marked read-only");
    		printf("\nWrite attempted to page marked read-only");
    		interrupt->Halt();
    		break;
		}
    		
    	case BusErrorException: {
			DEBUG('a', "\nTranslation resulted in an invalid physical address");
    		printf("\nTranslation resulted in an invalid physical address");
    		interrupt->Halt();
    		break;
		}
    		
    	case AddressErrorException: {
			DEBUG('a', "\nUnaligned reference or one that was beyond the end of the address space");
    		printf("\nUnaligned reference or one that was beyond the end of the address space");
    		interrupt->Halt();
    		break;
		}
    		
    	case OverflowException: {
			DEBUG('a', "\nInteger overflow in add or sub");
    		printf("\nInteger overflow in add or sub");
    		interrupt->Halt();
    		break;
		}
    		

    	case IllegalInstrException: {
			DEBUG('a', "\nUnimplemented or reserved instr");
    		printf( "\nUnimplemented or reserved instr");
    		interrupt->Halt();
    		break;
		}
    		

    	case NumExceptionTypes: {
			DEBUG('a', "\nNum exception types");
    		printf("\nNum exception types");
    		interrupt->Halt();
    		break;
		}
    		
    }
}
