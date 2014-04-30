#include <iostream>
#include <stdio.h>
#include "src/Monitor.hpp"

int main(int argc, char *argv[])
{
	Monitor *monitor = new Monitor();	

	Mutex *m = new Mutex(1);			
	
	monitor->lock(m);
	/*char x[] = "Hi";
	m->setDataForReturn(x,sizeof(x));*/
	monitor->log(INFO,"CS");
	monitor->unlock(m);	
		
	while(true);	
	monitor->finalize();	
	delete monitor;
	monitor = NULL;
	return 0;
}
