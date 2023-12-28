#include "syscall.h"

int main()
{
	int pingID, pongID, semPing, semPong;

	PrintString("Semaphore Ping-Pong Application\n\n");

	semPing = CreateSemaphore("ping2", 1);
	semPong = CreateSemaphore("pong2", 0);

	pingID = Exec("./test/ping2");
	pongID = Exec("./test/pong2");

	Join(pingID);
	Join(pongID);
}
