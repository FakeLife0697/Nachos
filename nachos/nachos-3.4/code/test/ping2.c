#include "syscall.h"

int main()
{
	int i;
	for (i = 0; i < 1000; i++)
	{
		Wait("ping2");
		PrintChar('A');
		Signal("pong2");
	}
}
