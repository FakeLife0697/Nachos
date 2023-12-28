#include "syscall.h"

int main()
{
	int i;
	for (i = 0; i < 1000; i++)
	{
		Wait("pong2");
		PrintChar('B');
		Signal("ping2");
	}
}
