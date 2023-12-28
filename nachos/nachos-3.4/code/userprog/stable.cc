/* ----------------------------------------------
 * Team member:
 * 21120223 - Dương Hải Đông
 * 21120240 - Nguyễn Vân Hào
 * 21120256 - Nguyễn Minh Huy
 * 21120257 - Tôn Anh Huy
 * ----------------------------------------------
 */

#include "stable.h"

// Constructor
STable::STable()
{	
	this->bm = new BitMap(MAX_SEMAPHORE);
	
	for(int i =0; i < MAX_SEMAPHORE; i++)
	{
		this->semTab[i] = NULL;
	}
}

// Destructor
STable::~STable()
{
	if(this->bm)
	{
		delete this->bm;
		this->bm = NULL;
	}
	for(int i=0; i < MAX_SEMAPHORE; i++)
	{
		if(this->semTab[i])
		{
			delete this->semTab[i];
			this->semTab[i] = NULL;
		}
	}
	
}

int STable::Create(char *name, int init)
{

	// Check semaphore
	for(int i=0; i<MAX_SEMAPHORE; i++)
	{
		if(bm->Test(i))
		{
			if(strcmp(name, semTab[i]->GetName()) == 0)
			{
				return -1;
			}
		}
		
	}
	// Find free slot
	int id = this->FindFreeSlot();
	
	// If not found
	if(id < 0)
	{
		return -1;
	}

	// If found
	this->semTab[id] = new Sem(name, init);
	return 0;
}

int STable::Wait(char *name)
{
	for(int i =0; i < MAX_SEMAPHORE; i++)
	{
		// Check semaphore
		if(bm->Test(i))
		{
			// Check semaphore name
			if(strcmp(name, semTab[i]->GetName()) == 0)
			{
				// If found, down(); 
				semTab[i]->wait();
				return 0;
			}
		}
	}
	printf("Khong ton tai semaphore");
	return -1;
}

int STable::Signal(char *name)
{
	for(int i =0; i < MAX_SEMAPHORE; i++)
	{
		// Check semaphore
		if(bm->Test(i))
		{
			// Check semaphore name
			if(strcmp(name, semTab[i]->GetName()) == 0)
			{
				// If found, up(); 
				semTab[i]->signal();
				return 0;
			}
		}
	}
	printf("Khong ton tai semaphore");
	return -1;
}

int STable::FindFreeSlot()
{
	return this->bm->Find();
}
