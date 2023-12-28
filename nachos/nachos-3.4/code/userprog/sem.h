/* ----------------------------------------------
 * Team member:
 * 21120223 - Dương Hải Đông
 * 21120240 - Nguyễn Vân Hào
 * 21120256 - Nguyễn Minh Huy
 * 21120257 - Tôn Anh Huy
 * ----------------------------------------------
 */

#ifndef SEM_H
#define SEM_H
#include "synch.h"
#include "bitmap.h"

// Lop Sem dung de quan ly semaphore.
class Sem
{
private:
	char name[50];		// Ten cua semaphore
	Semaphore* sem;		// Tao semaphore de quan ly
public:
	// Khoi tao doi tuong Sem. Gan gia tri ban dau la null
	// Nho khoi tao sem su dung
	Sem(char* na, int i)
	{
		strcpy(this->name, na);
		sem = new Semaphore(this->name, i);
	}

	~Sem()
	{
		if(sem)
			delete sem;
	}

	void wait()
	{
		sem->P();	// Down(sem)
	}

	void signal()
	{
		sem->V();	// Up(sem)
	}
	
	char* GetName()
	{
		return this->name;
	}
};

#endif // SEM_H
