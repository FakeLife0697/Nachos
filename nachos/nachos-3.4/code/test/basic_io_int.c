#include "syscall.h"


int main() {

	int i;
	PrintString("\nInput your number: \n");
	i = ReadInt();
	PrintString("Your number is: \n");
	PrintInt(i);
		
	Halt();
	return 0;
}
