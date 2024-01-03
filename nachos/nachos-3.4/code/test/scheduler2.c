#include "syscall.h"

int main()
{
	int pingID;
	int pongID;
	int semPong;
	int semPing;

	PrintString(" Test Ping Pong program...:\n\n");

	// tao 2 semaphore
	semPing = CreateSemaphore("ping2", 1);
	semPong = CreateSemaphore("pong2", 0);

	// tao 2 tien trinh
	pingID = Exec("./test/ping2");
	pongID = Exec("./test/pong2");

	// cho 2 tien trinh chay xen ke nhau
	Join(pingID);
	Join(pongID);
}
