#include "syscall.h"


int main() {
    int file_id;

	Create("new_file.txt");
    PrintString("\nSTATUS: Creating file...");
    file_id = Open("new_file.txt", 0);
    PrintString("\nSTATUS: Opening file...");
    Write("some content just added", 23, file_id);
    PrintString("\nSTATUS: Writing file...");
    Close(file_id);
    PrintString("\nSTATUS: Closing file...");
	Halt();
	return 0;
}
