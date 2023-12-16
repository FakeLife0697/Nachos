#include "syscall.h"



int main()
{

    int i = 0;
    PrintString("\n===== ASCII Table =====\n");

    for (i ; i <= 127; i++) {

        PrintString("Decimal ");
        PrintInt(i);
        PrintString(": ");
        PrintChar((char)i);
        PrintString("\n");
    }

    PrintString("+--------+----------+\n");

    Halt();
    return 0;
}