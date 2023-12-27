#include "syscall.h"


void main()
{
	int pingID, pongID;
	PrintString("Ping-Pong program...\n\n");
	pingID = Exec("./test/ping");
	pongID = Exec("./test/pong");
	Join(pingID);
	Join(pongID);	
}
