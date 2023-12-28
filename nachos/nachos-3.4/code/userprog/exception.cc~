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
	// Input: arg1: Dia chi cua chuoi name, arg2: type
			// Output: Tra ve OpenFileID neu thanh, -1 neu loi
			// Chuc nang: Tra ve ID cua file.
	 
			//OpenFileID Open(char *name, int type)
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
			int type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
			char* filename;
			filename = User2System(virtAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space voi bo dem name dai MaxFileLength
			//Kiem tra xem OS con mo dc file khong
			
			// update 4/1/2018
			int freeSlot = fileSystem->FindFreeSlot();
			if (freeSlot != -1) //Chi xu li khi con slot trong
			{
				if (type == 0 || type == 1) //chi xu li khi type = 0 hoac 1
				{
					
					if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
					{
						machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
					}
				}
				else if (type == 2) // xu li stdin voi type quy uoc la 2
				{
					machine->WriteRegister(2, 0); //tra ve OpenFileID
				}
				else // xu li stdout voi type quy uoc la 3
				{
					machine->WriteRegister(2, 1); //tra ve OpenFileID
				}
				delete[] filename;
				return;
			}
			machine->WriteRegister(2, -1); //Khong mo duoc file return -1
			
			delete[] filename;
			return;
	
}

void SC_Close_execution() {
	//Input id cua file(OpenFileID)
			// Output: 0: thanh cong, -1 that bai
			int fid = machine->ReadRegister(4); // Lay id cua file tu thanh ghi so 4
			if (fid >= 0 && fid <= 14) //Chi xu li khi fid nam trong [0, 14]
			{
				if (fileSystem->openf[fid]) //neu mo file thanh cong
				{
					delete fileSystem->openf[fid]; //Xoa vung nho luu tru file
					fileSystem->openf[fid] = NULL; //Gan vung nho NULL
					machine->WriteRegister(2, 0);
					return;
				}
			}
			machine->WriteRegister(2, -1);
			return;
}

void SC_Read_execution() {

/*
	int Read(char *buffer, int size, OpenFileId id);
	Input: 
		- buffer: chua ky tu read duoc
		- size: so luong ky tu can doc
		- id: id cua file can duoc 
	Output:
		Loi: -1
		Thanh cong: -2 (file rong)
		Thanh cong: so byte doc duoc
*/

	int virtAddr = machine->ReadRegister(4);
	int size = machine->ReadRegister(5);
	int file_id = machine->ReadRegister(6);
	int oldPos;
	int newPos;
	char* buf;

	// Kiem tra file_id hop le [0, 9]
	if (file_id < 0 || file_id > 9) {

		// Loi: file_id khong hop le
		machine->WriteRegister(2, -1);
		return;
	}
	// Kiem tra file_id ton tai
	if (fileSystem->openf[file_id] == NULL) {

		// Loi: file khong ton tai
		machine->WriteRegister(2, -1);
		return;
	}
	// Kiem tra co phai `stdout` (type = 3)
	if (fileSystem->openf[file_id]->type == 3) {

		// Loi: khong the read file stdout
		machine->WriteRegister(2, -1);
		return;
	}

	oldPos = fileSystem->openf[file_id]->GetCurrentPos();
	buf = User2System(virtAddr, size); // copy chuoi tu User sang System
	// TH: `stdin` (type = 2)
	if (fileSystem->openf[file_id]->type == 2) {

		int actual_size = gSynchConsole->Read(buf, size);
		System2User(virtAddr, actual_size, buf);
		machine->WriteRegister(2, actual_size);
		delete buf;
		return;
	}
	// TH: doc file bth (tra ve so byte that su)

	// Doc `size` bytes, luu vao `buf` (Tra ve # bytes that su doc duoc)
	if ((fileSystem->openf[file_id]->Read(buf, size))  > 0) {

		newPos = fileSystem->openf[file_id]->GetCurrentPos();
		// actual size = newPos - oldPos

		System2User(virtAddr, newPos - oldPos, buf);
		machine->WriteRegister(2, newPos - oldPos);
	}
	// Doc file rong
	else {

		machine->WriteRegister(2, -2);
	}
	delete buf;
	return;
}

void SC_Write_execution() {
/*
	void Write(char *buffer, int size, OpenFileId id)
	Input: 
		- buffer: chua ky tu can ghi
		- size: so luong ky tu can ghi
		- id: id cua file can ghi 
	Output:
		Loi: -1
		Thanh cong: so byte doc duoc
*/

	int virtAddr = machine->ReadRegister(4);
	int size = machine->ReadRegister(5);
	int file_id = machine->ReadRegister(6);
	int oldPos;
	int newPos;
	char* buf;

	// Kiem tra file_id hop le [0, 9]
	if (file_id < 0 || file_id > 9) {

		// Loi: file_id khong hop le
		machine->WriteRegister(2, -1);
		return;
	}
	// Kiem tra file_id ton tai
	if (fileSystem->openf[file_id] == NULL) {

		// Loi: file khong ton tai
		machine->WriteRegister(2, -1);
		return;
	}

	// Kiem tra `read-only` (type = 1) & `stdin` (type = 2)
	int type = fileSystem->openf[file_id]->type;
	if (type == 1 || type == 2) {

		// Loi:
		machine->WriteRegister(2, -1);
		return;
	}

	oldPos = fileSystem->openf[file_id]->GetCurrentPos();
	buf = User2System(virtAddr, size); // Copy chuoi can write tu User qua System

	// TH: `read-write`
	if (type == 0) {

		if ((fileSystem->openf[file_id]->Write(buf, size)) > 0) {

			newPos = fileSystem->openf[file_id]->GetCurrentPos();
			machine->WriteRegister(2, newPos - oldPos);
		}
	}
	// TH: `stdout`
	if (type == 3) {
		int i = 0;
		while (buf[i] != '\0' && buf[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
		{
			gSynchConsole->Write(buf + i, 1); // Su dung ham Write cua lop SynchConsole 
			i++;
		}
		buf[i] = '\n';
		gSynchConsole->Write(buf + i, 1); // Write ky tu '\n'
		machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
	
	}
	delete buf;
	return;
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

void SC_Exec_execution() {
	// Đọc địa chỉ tên chương trình “name” từ thanh ghi r4. 
	int addr;
	addr = machine->ReadRegister(4);

	// Tên chương trình lúc này đang ở trong user space.
	// Gọi hàm User2System đã được khai báo trong lớp machine để chuyển vùng nhớ user space tới vùng nhớ system space. 
	char* name;
	name = User2System(addr, MaxFileLength + 1);

	if(name == NULL)
	{
		DEBUG('a', "\n Not enough memory in System");
		printf("\n Not enough memory in System");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	// Nếu bị lỗi thì báo “Không mở được file” và gán -1 vào thanh ghi 2. 
	OpenFile* opFile = fileSystem->Open(name);
	if (opFile == NULL)
	{
		printf("SC_Exec:: Can't open file.");
		machine->WriteRegister(2, -1);
	}
	// Nếu không có lỗi thì gọi pTab-> ExecUpdate(name), 
	// trả về và lưu kết quả thực thi phương thức này vào thanh ghi r2.
	else
	{
		int tmp = pTab->ExecUpdate(name);
		machine->WriteRegister(2, tmp);
	}

	delete opFile;
	delete[] name;
}

void SC_Join_execution() {
	// Đọc id của tiến trình cần Join từ thanh ghi r4. 
	int id;
	id = machine->ReadRegister(4);

	// Gọi thực hiện pTab->JoinUpdate(id) và lưu kết quả thực hiện của hàm vào thanh ghi r2.
	int tmp;
	tmp = pTab->JoinUpdate(id);

	machine->WriteRegister(2, tmp);
}

void SC_Exit_execution() {
	// Đọc exitStatus từ thanh ghi r4 
	int exitStatus;
	exitStatus = machine->ReadRegister(4);

	// Gọi thực hiện pTab->ExitUpdate(exitStatus)
	// và lưu kết quả thực hiện của hàm vào thanh ghi r2.
	int tmp;
	tmp = pTab->ExitUpdate(exitStatus);

	machine->WriteRegister(2, tmp);

	currentThread->FreeSpace();
	currentThread->Finish();
}

void SC_CreateSemaphore_execution() {
	// Đọc địa chỉ “name” từ thanh ghi r4. 
	int addr;
	addr = machine->ReadRegister(4);

	// Đọc giá trị “semval” từ thanh ghi r5. 
	int semval;
	semval = machine->ReadRegister(5);

	// Tên địa chỉ “name” lúc này đang ở trong user space.
	// Gọi hàm User2System đã được khai báo trong lớp machine để chuyển vùng nhớ user space tới vùng nhớ system space. 
	char* name = User2System(addr, MaxFileLength + 1);
	if(name == NULL)
	{
		DEBUG('a', "\n Not enough memory in System");
		printf("\n Not enough memory in System");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	// Gọi thực hiện hàm semTab->Create(name,semval) để tạo Semaphore, nếu có lỗi thì báo lỗi.
	int tmp;
	tmp = semTab->Create(name, semval);
	if(tmp == -1)
	{
		DEBUG('a', "\n Khong the khoi tao semaphore");
		printf("\n Khong the khoi tao semaphore");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;				
	}
 
	// Lưu kết quả thực hiện vào thanh ghi r2.
	machine->WriteRegister(2, tmp);

	delete[] name;
}

void SC_Signal_execution() {
	// Đọc địa chỉ “name” từ thanh ghi r4. 
	int addr;
	addr = machine->ReadRegister(4);
	
	// Tên địa chỉ “name” lúc này đang ở trong user space. 
	// Gọi hàm User2System đã được khai báo trong lớp machine để chuyển vùng nhớ user space tới vùng nhớ system space. 
	char* name;
	name = User2System(addr, MaxFileLength + 1);

	if(name == NULL)
	{
		DEBUG('a', "\n Not enough memory in System");
		printf("\n Not enough memory in System");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	// Gọi phương thức Signal() của lớp Stable.  
	int tmp;
	tmp = semTab->Signal(name);

	// Kiểm tra Semaphore “name” này có trong bảng sTab chưa, nếu chưa có thì báo lỗi.
	if(tmp == -1)
	{
		DEBUG('a', "\n Khong ton tai ten semaphore nay!");
		printf("\n Khong ton tai ten semaphore nay!");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;				
	}

	// Lưu kết quả thực hiện vào thanh ghi r2. 
	machine->WriteRegister(2, tmp);

	delete[] name;
}

void SC_Wait_execution() {
	// Đọc địa chỉ “name” từ thanh ghi r4. 
	int addr;
	addr = machine->ReadRegister(4);
	
	// Tên địa chỉ “name” lúc này đang ở trong user space. 
	// Gọi hàm User2System đã được khai báo trong lớp machine để chuyển vùng nhớ user space tới vùng nhớ system space. 
	char* name;
	name = User2System(addr, MaxFileLength + 1);

	if(name == NULL)
	{
		DEBUG('a', "\n Not enough memory in System");
		printf("\n Not enough memory in System");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	// Gọi phương thức Wait() của lớp Stable.  
	int tmp;
	tmp = semTab->Wait(name);

	// Kiểm tra Semaphore “name” này có trong bảng sTab chưa, nếu chưa có thì báo lỗi.
	if(tmp == -1)
	{
		DEBUG('a', "\n Khong ton tai ten semaphore nay!");
		printf("\n Khong ton tai ten semaphore nay!");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;				
	}

	// Lưu kết quả thực hiện vào thanh ghi r2. 
	machine->WriteRegister(2, tmp);

	delete[] name;
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

				case SC_Open: {
					SC_Open_execution();
					break;
				}

				case SC_Close: {
					SC_Close_execution();
					break;
				}

				case SC_Read: {

					SC_Read_execution();
					break;
				}

				case SC_Write: {

					SC_Write_execution();
					break;
				}

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

				case SC_Exec: {
					SC_Exec_execution();
					break;
				}

				case SC_Join: {
					SC_Join_execution();
					break;
				}
				
				case SC_Exit: {
					SC_Exit_execution();
					break;
				}

				case SC_CreateSemaphore: {
					SC_CreateSemaphore_execution();
					break;
				}

				case SC_Signal: {
					SC_Signal_execution();
					break;
				}

				case SC_Wait: {
					SC_Wait_execution();
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
