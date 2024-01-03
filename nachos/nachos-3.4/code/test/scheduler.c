#include "syscall.h"

int main()
{
	int pingID;
	int pongID;
	int semPong;
	int semPing;

	PrintString(" Test Ping Pong program...:\n\n");

	// tao 2 semaphore
	semPing = CreateSemaphore("ping", 1);
	semPong = CreateSemaphore("pong", 0);

	// tao 2 tien trinh
	pingID = Exec("./test/ping");
	pongID = Exec("./test/pong");

	// cho 2 tien trinh chay xen ke nhau
	
	Join(pingID);
	Join(pongID);	
	
}
