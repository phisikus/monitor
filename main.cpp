#include <iostream>
#include <unistd.h>
#include "src/Monitor.hpp"


void tryPowerOfTwo()
{
	Mutex *m = new Mutex(1);
	Monitor *monitor = new Monitor();
	
	for(unsigned int i = 0; i < 2 ; i++)
	{
		monitor->lock(m);
		if(m->getDataSize() == 0)
		{
			int x = 2;
			m->setDataForReturn(&x,sizeof(x));
		}
		int *y = (int *) m->getData();

		monitor->log(INFO,"CS: " + to_string(*y));
		(*y) *= 2;
		monitor->unlock(m);
	}

	while(true);
	monitor->finalize();
	delete monitor;
	monitor = NULL;


}


void tryCondition()
{
	Mutex *m = new Mutex(1);
	ConditionVariable *cv = new ConditionVariable(1);
	Monitor *monitor = new Monitor();
	

	if(monitor->communicator->processId % 2)
	{
		monitor->lock(m);		
		monitor->wait(cv, m);	
		monitor->unlock(m);
	}    
	else
	{
		usleep(500000);
		monitor->signalOne(cv);				
	}

	while(true);
	monitor->finalize();
	delete monitor;
	monitor = NULL;


}

int main(int argc, char *argv[])
{
	tryPowerOfTwo();
	//tryCondition();
	return 0;
}
