#include "syscall.h"


int main() {

	char c;
	PrintString("\nInput your char: \n");
	c = ReadChar();
	PrintString("Your char is: \n");
	PrintChar(c);
	
	
	Halt();
	return 0;
}
