#include "syscall.h"


int main() {
    	
	char* str;
	PrintString("\nInput your string: \n");
	ReadString(str, 10); // change 10 to another length if you want
	PrintString("Your string is: \n");
	PrintString(str);
	
	Halt();
	return 0;
}
