#include <iostream>
#include <unistd.h>
#include "src/Monitor.hpp"

int main(int argc, char *argv[])
{
	Monitor *monitor = new Monitor();	

	Mutex *m = new Mutex(1);			
	
	monitor->lock(m);
	if(m->getDataSize() == 0)
	{
		int x = 2;
		m->setDataForReturn(&x,sizeof(x));
	}	
	int *y = (int *) m->getData();
	
	//usleep(1000000);
	monitor->log(INFO,"CS: " + to_string(*y));
	(*y) *= 2;
	m->setDataForReturn(y,sizeof(*y));
	monitor->unlock(m);
		
	while(true);	
	monitor->finalize();	
	delete monitor;
	monitor = NULL;
	return 0;
}
