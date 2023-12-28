#include "syscall.h"

int main()
{
	int pingID, pongID;
	PrintString("Normal Ping-Pong application\n\n");

	pingID = Exec("./test/ping");
	pongID = Exec("./test/pong");

	Join(pingID);
	Join(pongID);	

	return 0;
}
