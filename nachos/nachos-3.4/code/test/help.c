#include "syscall.h"


int main() {

    PrintString("\n\n\t========== MEMBERS INFORMATION ==========");
    PrintString("\n\tSTUDENT ID                    NAME");
    PrintString("\n\t21120223                      Duong Hai Dong");
    PrintString("\n\t21120240                      Nguyen Van Hao");
    PrintString("\n\t21120256                      Nguyen Minh Huy");
    PrintString("\n\t21120257                      Ton Anh Huy");
	

    PrintString("\n\n\t========== PROGRAMS INSTRUCTION ==========");
    PrintString("\n\t1. sort - User will input up to 100 integer then sort base on Buble Sort algorithm");
    PrintString("\n\tcommand line: ./userprog/nachos -rs 1023 -x ./test/bubble_sort\n");
    PrintString("\n\t2. ascii - print ASCII table");
    PrintString("\n\tcommand line: ./userprog/nachos -rs 1023 -x ./test/ascii\n");
	Halt();
	return 0;
}
