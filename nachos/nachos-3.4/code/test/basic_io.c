#include "syscall.h"


int main() {

	//uncomment to test I/O for interger
	// int i;
	// PrintString("\nInput your number: \n");
	// i = ReadInt();
	// PrintString("Your number is: \n");
	// PrintInt(i);
	
	
	// uncomment to test I/O for char
	// char c;
	// PrintString("\nInput your char: \n");
	// c = ReadChar();
	// PrintString("Your char is: \n");
	// PrintChar(c);
	
	char* str;
	PrintString("\nInput your string: \n");
	ReadString(str, 10); // change 10 to another length if you want
	PrintString("Your string is: \n");
	PrintString(str);
	
	Halt();
	return 0;
}
